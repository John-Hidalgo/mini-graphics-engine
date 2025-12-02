#version 330 core
in vec2 fragCoord;
out vec4 FragColor;

uniform vec2 iResolution;
uniform float iTime;
uniform vec3 cameraPos;
uniform vec3 cameraTarget;
uniform int numCubes;
uniform vec3 cubeCenters[10];
uniform vec3 cubeSizes[10];
uniform vec3 cubeColors[10];
uniform float cubeRefractions[10];
uniform int numSpheres;
uniform vec3 sphereCenters[10];
uniform float sphereRadii[10];
uniform vec3 sphereColors[10];
uniform float sphereRefractions[10];

const float REFRACTIVE_INDEX = 1.5;
const float MAX_DIST = 100.0;
const float EPSILON = 0.001;
const int MAX_STEPS = 64;
const int SHADOW_STEPS = 32;
const int GI_STEPS = 32;
const int GI_BOUNCES = 3;
const int GI_SAMPLES = 4;

float hash(float n) {
	return fract(sin(n) * 43758.5453123);
}

float hash2(vec2 p) {
	return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

vec3 hash3(vec3 p) {
	p = vec3(dot(p, vec3(127.1, 311.7, 74.7)),
			 dot(p, vec3(269.5, 183.3, 246.1)),
			 dot(p, vec3(113.5, 271.9, 124.6)));
	return fract(sin(p) * 43758.5453123);
}

vec3 randomCosineDirection(vec3 normal, vec2 seed) {
	vec3 rand = hash3(vec3(seed, iTime));
	float u = rand.x;
	float v = rand.y;
	
	float r = sqrt(u);
	float theta = 2.0 * 3.14159265 * v;
	
	vec3 tangent = abs(normal.y) < 0.999 ?
		normalize(cross(normal, vec3(0.0, 1.0, 0.0))) :
		normalize(cross(normal, vec3(1.0, 0.0, 0.0)));
	vec3 bitangent = cross(normal, tangent);
	
	vec3 dir = tangent * (r * cos(theta)) +
			   bitangent * (r * sin(theta)) +
			   normal * sqrt(1.0 - u);
	
	return normalize(dir);
}

float sdSphere(vec3 p, vec3 center, float radius) {
	return length(p - center) - radius;
}

float sdBox(vec3 p, vec3 center, vec3 size) {
	vec3 q = abs(p - center) - size * 0.5;
	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sdPlane(vec3 p, float height) {
	return p.y + height;
}

struct SceneData {
	float dist;
	vec3 color;
	float refraction;
	int materialType;
};

SceneData opUnion(SceneData d1, SceneData d2) {
	return d1.dist < d2.dist ? d1 : d2;
}

SceneData mapScene(vec3 p) {
	SceneData scene;
	scene.dist = sdPlane(p, 2.0);
	scene.color = vec3(0.7);
	scene.refraction = 0.0;
	scene.materialType = 3;
	
	for (int i = 0; i < numSpheres; i++) {
		SceneData sphere;
		sphere.dist = sdSphere(p, sphereCenters[i], sphereRadii[i]);
		sphere.color = sphereColors[i];
		sphere.refraction = sphereRefractions[i];
		
		if (sphere.color == vec3(0.2, 1.0, 0.2)) {
			sphere.materialType = 1;
		} else if (sphere.refraction > 0.0) {
			sphere.materialType = 2;
		} else {
			sphere.materialType = 0;
		}
		
		scene = opUnion(scene, sphere);
	}
	
	for (int i = 0; i < numCubes; i++) {
		SceneData cube;
		cube.dist = sdBox(p, cubeCenters[i], cubeSizes[i]);
		cube.color = cubeColors[i];
		cube.refraction = cubeRefractions[i];
		
		if (cube.refraction > 0.0) {
			cube.materialType = 2;
		} else if (cube.color == vec3(0.2, 0.2, 1.0)) {
			cube.materialType = 0;
		} else {
			cube.materialType = 1;
		}
		
		scene = opUnion(scene, cube);
	}
	
	return scene;
}

vec3 calcNormal(vec3 p) {
	vec2 e = vec2(EPSILON, 0.0);
	return normalize(vec3(
		mapScene(p + e.xyy).dist - mapScene(p - e.xyy).dist,
		mapScene(p + e.yxy).dist - mapScene(p - e.yxy).dist,
		mapScene(p + e.yyx).dist - mapScene(p - e.yyx).dist
	));
}

bool rayMarch(vec3 ro, vec3 rd, out float t, out SceneData data, int maxSteps) {
	t = 0.0;
	
	for (int i = 0; i < maxSteps; i++) {
		vec3 p = ro + rd * t;
		data = mapScene(p);
		
		if (abs(data.dist) < EPSILON) {
			return true;
		}
		
		if (t > MAX_DIST) {
			return false;
		}
		
		t += data.dist;
	}
	
	return false;
}

bool rayMarch(vec3 ro, vec3 rd, out float t, out SceneData data) {
	return rayMarch(ro, rd, t, data, MAX_STEPS);
}

float calculateSoftShadow(vec3 ro, vec3 rd, float mint, float maxt, float k) {
	float res = 1.0;
	float t = mint;
	
	for (int i = 0; i < SHADOW_STEPS; i++) {
		float h = mapScene(ro + rd * t).dist;
		res = min(res, k * h / t);
		t += clamp(h, 0.01, 0.2);
		if (res < 0.001 || t > maxt) break;
	}
	
	return clamp(res, 0.0, 1.0);
}

vec3 calculateDirectLighting(vec3 p, vec3 n, vec3 albedo, vec3 lightPos) {
	vec3 lightDir = normalize(lightPos - p);
	float lightDist = length(lightPos - p);
	
	float shadow = calculateSoftShadow(p + n * EPSILON * 2.0, lightDir, 0.01, lightDist, 8.0);
	float attenuation = 1.0 / (1.0 + 0.1 * lightDist * lightDist);
	
	float diffuse = max(dot(n, lightDir), 0.0);
	vec3 ambient = vec3(0.05);
	
	vec3 lighting = albedo * (ambient + diffuse * shadow * attenuation);
	
	return lighting;
}

vec3 calculateGlobalIllumination(vec3 p, vec3 n, vec3 albedo, vec2 seed) {
	vec3 giColor = vec3(0.0);
	
	for (int i = 0; i < GI_SAMPLES; i++) {
		vec2 sampleSeed = seed + vec2(float(i) * 13.7, float(i) * 17.3);
		vec3 rayDir = randomCosineDirection(n, sampleSeed);
		
		float t;
		SceneData data;
		
		if (rayMarch(p + n * EPSILON * 2.0, rayDir, t, data, GI_STEPS)) {
			vec3 hitPos = p + rayDir * t;
			vec3 hitNormal = calcNormal(hitPos);
			
			vec3 lightPos = vec3(sin(iTime) * 5.0, 2.0 + cos(iTime * 0.7) * 2.0, -3.0);
			vec3 indirectLight = calculateDirectLighting(hitPos, hitNormal, data.color, lightPos);
			
			float cosTheta = max(dot(n, rayDir), 0.0);
			giColor += indirectLight * cosTheta;
		} else {
			giColor += vec3(0.1, 0.1, 0.2) * max(dot(n, rayDir), 0.0);
		}
	}
	
	return giColor * albedo / float(GI_SAMPLES);
}

vec3 refractRay(vec3 incident, vec3 normal, float eta) {
	float cosi = dot(-incident, normal);
	float k = 1.0 - eta * eta * (1.0 - cosi * cosi);
	if (k < 0.0) {
		return reflect(incident, normal);
	}
	return eta * incident + (eta * cosi - sqrt(k)) * normal;
}

vec3 traceRay(vec3 ro, vec3 rd, vec3 lightPos) {
	vec3 color = vec3(0.0);
	vec3 throughput = vec3(1.0);
	
	for (int bounce = 0; bounce < 4; bounce++) {
		float t;
		SceneData data;
		
		if (!rayMarch(ro, rd, t, data)) {
			color += vec3(0.1, 0.1, 0.2) * throughput;
			break;
		}
		
		vec3 p = ro + rd * t;
		vec3 n = calcNormal(p);
		
		if (data.materialType == 2) {
			float R0 = pow((1.0 - REFRACTIVE_INDEX) / (1.0 + REFRACTIVE_INDEX), 2.0);
			float cosTheta = dot(-rd, n);
			float fresnel = R0 + (1.0 - R0) * pow(1.0 - cosTheta, 5.0);
			
			if (hash2(fragCoord + float(bounce)) < fresnel) {
				rd = reflect(rd, n);
				ro = p + n * EPSILON * 2.0;
			} else {
				rd = refractRay(rd, n, 1.0 / REFRACTIVE_INDEX);
				ro = p - n * EPSILON * 2.0;
			}
			throughput *= data.color;
			
		} else if (data.materialType == 1) {
			vec3 directLight = calculateDirectLighting(p, n, data.color, lightPos);
			color += directLight * throughput * 0.2;
			
			rd = reflect(rd, n);
			float roughness = (data.color == vec3(0.7, 0.7, 0.7)) ? 0.007 : 0.0;
			if (roughness > 0.0) {
				vec3 rand = hash3(vec3(fragCoord, float(bounce)));
				rd = normalize(rd + (rand * 2.0 - 1.0) * roughness);
			}
			ro = p + n * EPSILON * 2.0;
			throughput *= data.color;
			
		} else {
			vec3 directLight = calculateDirectLighting(p, n, data.color, lightPos);
			color += directLight * throughput;
			
			if (data.materialType != 3) {
				vec3 giLight = calculateGlobalIllumination(p, n, data.color, fragCoord + float(bounce));
				color += giLight * throughput * 0.8;
			}
			break;
		}
		
		if (bounce > 1) {
			float p_continue = max(throughput.r, max(throughput.g, throughput.b));
			if (hash2(fragCoord + float(bounce) * 7.3) > p_continue) {
				break;
			}
			throughput /= p_continue;
		}
	}
	
	return color;
}

void generateCameraRay(vec2 uv, out vec3 ro, out vec3 rd) {
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 cameraDir = normalize(cameraTarget - cameraPos);
	vec3 cameraRight = normalize(cross(up, cameraDir));
	vec3 cameraUp = cross(cameraDir, cameraRight);
	
	ro = cameraPos;
	rd = normalize(cameraDir + uv.x * cameraRight + uv.y * cameraUp);
}

vec3 applyToneMapping(vec3 color) {
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(0.4545));
	return color;
}

void main() {
	vec3 lightPos = vec3(sin(iTime) * 5.0, 2.0 + cos(iTime * 0.7) * 2.0, -3.0);
	
	vec2 uv = (fragCoord * 0.5 + 0.5) * 2.0 - 1.0;
	uv.x *= iResolution.x / iResolution.y;
	
	vec3 ro, rd;
	generateCameraRay(uv, ro, rd);
	
	vec3 color = traceRay(ro, rd, lightPos);
	color = applyToneMapping(color);
	
	FragColor = vec4(color, 1.0);
}
