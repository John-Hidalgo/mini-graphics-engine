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
uniform int numTriangles;
uniform vec3 triangleV0[10];
uniform vec3 triangleV1[10];
uniform vec3 triangleV2[10];
uniform vec3 triangleColors[10];
uniform float triangleRefractions[10];

const float REFRACTIVE_INDEX = 1.5;
const int SSS_SPHERE_INDEX = 2;
const vec3 SSS_COLOR = vec3(1.0, 0.2, 0.2);

bool intersectCube(vec3 rayOrigin, vec3 rayDirection, vec3 cubeCenter, vec3 cubeSize,
				   out float distance, out vec3 position, out vec3 normal, out vec3 color,
				   out float refraction, int index) {
	vec3 minBounds = cubeCenter - cubeSize * 0.5;
	vec3 maxBounds = cubeCenter + cubeSize * 0.5;
	vec3 invDir = 1.0 / rayDirection;
	vec3 t1 = (minBounds - rayOrigin) * invDir;
	vec3 t2 = (maxBounds - rayOrigin) * invDir;
	vec3 tmin = min(t1, t2);
	vec3 tmax = max(t1, t2);
	
	float tNear = max(max(tmin.x, tmin.y), tmin.z);
	float tFar = min(min(tmax.x, tmax.y), tmax.z);
	
	if (tNear <= tFar && tFar > 0.0) {
		distance = tNear;
		position = rayOrigin + rayDirection * tNear;
		color = cubeColors[index];
		refraction = cubeRefractions[index];
		
		vec3 localPos = (position - cubeCenter) / cubeSize;
		vec3 absPos = abs(localPos);
		if (absPos.x > absPos.y && absPos.x > absPos.z) {
			normal = vec3(sign(localPos.x), 0.0, 0.0);
		} else if (absPos.y > absPos.z) {
			normal = vec3(0.0, sign(localPos.y), 0.0);
		} else {
			normal = vec3(0.0, 0.0, sign(localPos.z));
		}
		return true;
	}
	return false;
}

bool intersectSphere(vec3 rayOrigin, vec3 rayDirection, vec3 sphereCenter, float radius,
					 out float distance, out vec3 position, out vec3 normal, out vec3 color,
					 out float refraction, int index) {
	vec3 oc = rayOrigin - sphereCenter;
	float a = dot(rayDirection, rayDirection);
	float b = 2.0 * dot(oc, rayDirection);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4.0 * a * c;
	
	if (discriminant > 0.0) {
		float t = (-b - sqrt(discriminant)) / (2.0 * a);
		if (t > 0.0) {
			distance = t;
			position = rayOrigin + rayDirection * t;
			normal = normalize(position - sphereCenter);
			color = sphereColors[index];
			refraction = sphereRefractions[index];
			return true;
		}
	}
	return false;
}

bool intersectTriangle(vec3 rayOrigin, vec3 rayDirection, vec3 v0, vec3 v1, vec3 v2,
					   out float distance, out vec3 position, out vec3 normal, out vec3 color,
					   out float refraction, int index) {
	vec3 edge1 = v1 - v0;
	vec3 edge2 = v2 - v0;
	vec3 h = cross(rayDirection, edge2);
	float a = dot(edge1, h);
	
	if (abs(a) < 0.0001) return false;
	
	float f = 1.0 / a;
	vec3 s = rayOrigin - v0;
	float u = f * dot(s, h);
	
	if (u < 0.0 || u > 1.0) return false;
	
	vec3 q = cross(s, edge1);
	float v = f * dot(rayDirection, q);
	
	if (v < 0.0 || u + v > 1.0) return false;
	
	float t = f * dot(edge2, q);
	if (t > 0.0001) {
		distance = t;
		position = rayOrigin + rayDirection * t;
		normal = normalize(cross(edge1, edge2));
		color = triangleColors[index];
		refraction = triangleRefractions[index];
		return true;
	}
	return false;
}

bool intersectFloor(vec3 rayOrigin, vec3 rayDirection, out float distance, out vec3 position,
					out vec3 normal, out vec3 color, out float refraction) {
	if (rayDirection.y < 0.0) {
		float t = (-2.0 - rayOrigin.y) / rayDirection.y;
		if (t > 0.0) {
			distance = t;
			position = rayOrigin + rayDirection * t;
			normal = vec3(0.0, 1.0, 0.0);
			color = vec3(0.7);
			refraction = 0.0;
			return true;
		}
	}
	return false;
}

bool intersectScene(vec3 rayOrigin, vec3 rayDirection, out float distance, out vec3 position,
					out vec3 normal, out vec3 color, out float refraction, out int objectIndex) {
	float closestDist = 1e10;
	vec3 closestPos, closestNormal, closestColor;
	float closestRefraction = 0.0;
	int closestIndex = -1;
	bool foundHit = false;
	
	float floorDist;
	vec3 floorPos, floorNormal, floorColor;
	float floorRefraction;
	if (intersectFloor(rayOrigin, rayDirection, floorDist, floorPos, floorNormal, floorColor, floorRefraction)) {
		if (floorDist < closestDist) {
			closestDist = floorDist;
			closestPos = floorPos;
			closestNormal = floorNormal;
			closestColor = floorColor;
			closestRefraction = floorRefraction;
			closestIndex = -1;
			foundHit = true;
		}
	}
	
	for (int i = 0; i < numCubes; i++) {
		float dist;
		vec3 pos, norm, col;
		float refrac;
		if (intersectCube(rayOrigin, rayDirection, cubeCenters[i], cubeSizes[i], dist, pos, norm, col, refrac, i)) {
			if (dist < closestDist) {
				closestDist = dist;
				closestPos = pos;
				closestNormal = norm;
				closestColor = col;
				closestRefraction = refrac;
				closestIndex = -1000 - i;
				foundHit = true;
			}
		}
	}
	
	for (int i = 0; i < numSpheres; i++) {
		float dist;
		vec3 pos, norm, col;
		float refrac;
		if (intersectSphere(rayOrigin, rayDirection, sphereCenters[i], sphereRadii[i], dist, pos, norm, col, refrac, i)) {
			if (dist < closestDist) {
				closestDist = dist;
				closestPos = pos;
				closestNormal = norm;
				closestColor = col;
				closestRefraction = refrac;
				closestIndex = i;
				foundHit = true;
			}
		}
	}
	
	for (int i = 0; i < numTriangles; i++) {
		float dist;
		vec3 pos, norm, col;
		float refrac;
		if (intersectTriangle(rayOrigin, rayDirection, triangleV0[i], triangleV1[i], triangleV2[i], dist, pos, norm, col, refrac, i)) {
			if (dist < closestDist) {
				closestDist = dist;
				closestPos = pos;
				closestNormal = norm;
				closestColor = col;
				closestRefraction = refrac;
				closestIndex = -2000 - i;
				foundHit = true;
			}
		}
	}
	
	if (foundHit) {
		distance = closestDist;
		position = closestPos;
		normal = closestNormal;
		color = closestColor;
		refraction = closestRefraction;
		objectIndex = closestIndex;
		return true;
	}
	return false;
}

vec3 refractRay(vec3 incident, vec3 normal, float eta) {
	float cosi = dot(-incident, normal);
	float k = 1.0 - eta * eta * (1.0 - cosi * cosi);
	if (k < 0.0) {
		return reflect(incident, normal);
	}
	return eta * incident + (eta * cosi - sqrt(k)) * normal;
}

void getMaterialProperties(vec3 color, float refraction, out float reflectivity, out float transparency) {
	float brightness = (color.r + color.g + color.b) / 3.0;
	
	if (color == vec3(1.0, 0.2, 0.2)) {
		reflectivity = 0.0;
		transparency = 0.0;
	} else if (color == vec3(0.2, 1.0, 0.2)) {
		reflectivity = 1.0;
		transparency = 0.0;
	} else if (color == vec3(0.7, 0.7, 0.7)) {
		reflectivity = 0.8;
		transparency = 0.0;
	} else if (color == vec3(0.2, 0.2, 1.0)) {
		reflectivity = 0.0;
		transparency = 0.0;
	} else {
		reflectivity = brightness * 0.6;
		transparency = 0.0;
	}
	
	if (refraction > 0.0) {
		transparency = refraction;
		reflectivity = 0.1;
	}
}

vec3 randomInUnitSphere() {
	return normalize(vec3(
		fract(sin(dot(fragCoord, vec2(12.9898, 78.233))) * 43758.5453),
		fract(sin(dot(fragCoord, vec2(39.346, 11.135))) * 87413.231),
		fract(sin(dot(fragCoord, vec2(73.156, 52.235))) * 52374.124)
	) * 2.0 - 1.0);
}

float calculateShadow(vec3 pos, vec3 lightPos) {
	vec3 lightDir = normalize(lightPos - pos);
	float shadowDist;
	vec3 shadowPos, shadowNormal, shadowColor;
	float shadowRefraction;
	int shadowIndex;
	
	if (intersectScene(pos + lightDir * 0.01, lightDir, shadowDist, shadowPos, shadowNormal, shadowColor, shadowRefraction, shadowIndex)) {
		float distToLight = length(lightPos - pos);
		if (shadowDist < distToLight) {
			return 0.3;
		}
	}
	return 1.0;
}

vec3 calculateSubsurfaceScattering(vec3 hitPosition, vec3 hitNormal, int sphereIndex) {
	if (sphereIndex != SSS_SPHERE_INDEX) {
		return vec3(0.0);
	}
	
	vec3 sphereCenter = sphereCenters[SSS_SPHERE_INDEX];
	float sphereRadius = sphereRadii[SSS_SPHERE_INDEX];
	vec3 lightOffset = vec3(
		cos(iTime * 0.8) * 0.6,
		sin(iTime * 1.2) * 0.5,
		sin(iTime * 0.6) * 0.6
	) * sphereRadius;
	vec3 internalLightPos = sphereCenter + lightOffset;
	
	vec3 toLightDir = normalize(internalLightPos - hitPosition);
	float distToLight = length(internalLightPos - hitPosition);
	float thickness = distToLight / sphereRadius;
	
	float attenuation = exp(-thickness * 3.5);
	float backLight = max(0.0, dot(-hitNormal, toLightDir));
	backLight = pow(backLight, 1.5);

	vec3 viewDir = normalize(cameraPos - hitPosition);
	float viewDot = dot(viewDir, hitNormal);
	float edgeFactor = 1.0 - abs(viewDot);
	edgeFactor = pow(edgeFactor, 2.0);
	
	vec3 sssColor = SSS_COLOR * backLight * attenuation * 4.5;
	float rim = pow(edgeFactor, 1.5);
	sssColor += SSS_COLOR * rim * 1.5 * attenuation;
	float hotspot = 1.0 / (1.0 + distToLight * distToLight * 0.5);
	sssColor += vec3(1.0, 0.6, 0.4) * hotspot * 2.0;
	
	return sssColor;
}

struct RayState {
	vec3 origin;
	vec3 direction;
	vec3 weight;
};

vec3 calculateDirectLighting(vec3 hitColor, vec3 hitNormal, vec3 hitPosition, vec3 lightPos, int objectIndex) {
	vec3 lightDir = normalize(lightPos - hitPosition);
	float diffuse = max(dot(hitNormal, lightDir), 0.0);
	float shadow = calculateShadow(hitPosition, lightPos);
	vec3 ambient = vec3(0.1);
	
	vec3 lighting = hitColor * (ambient + diffuse * shadow);
	
	if (objectIndex == SSS_SPHERE_INDEX) {
		lighting += calculateSubsurfaceScattering(hitPosition, hitNormal, objectIndex);
	}
	
	return lighting;
}

bool handleTransparentMaterial(inout RayState ray, vec3 hitPosition, vec3 hitNormal,
								vec3 hitColor, vec3 rayDirection, int bounce) {
	if (bounce >= 5) return false;
	
	float R0 = pow((1.0 - REFRACTIVE_INDEX) / (1.0 + REFRACTIVE_INDEX), 2.0);
	float cosTheta = dot(-rayDirection, hitNormal);
	float fresnel = R0 + (1.0 - R0) * pow(1.0 - cosTheta, 5.0);
	
	if (fresnel > 0.1) {
		ray.origin = hitPosition + hitNormal * 0.01;
		ray.direction = reflect(rayDirection, hitNormal);
		ray.weight *= hitColor * fresnel;
	}
	else {
		vec3 refractDir = refractRay(rayDirection, hitNormal, 1.0 / REFRACTIVE_INDEX);
		ray.origin = hitPosition - hitNormal * 0.01;
		ray.direction = refractDir;
		ray.weight *= hitColor * (1.0 - fresnel);
	}
	return true;
}

bool handleReflectiveMaterial(inout RayState ray, inout vec3 accumulatedColor,
							   vec3 hitPosition, vec3 hitNormal, vec3 hitColor,
							   vec3 rayDirection, float reflectivity, vec3 lightPos, int bounce, int objectIndex) {
	if (bounce >= 5) return false;
	
	vec3 directLighting = calculateDirectLighting(hitColor, hitNormal, hitPosition, lightPos, objectIndex);
	accumulatedColor += directLighting * ray.weight * (1.0 - reflectivity);
	
	ray.origin = hitPosition + hitNormal * 0.01;
	vec3 reflectDir = reflect(rayDirection, hitNormal);
	
	float roughness = 0.0;
	if (hitColor == vec3(0.7, 0.7, 0.7)) {
		roughness = 0.007;
	}
	
	if (roughness > 0.0) {
		reflectDir = normalize(reflectDir + randomInUnitSphere() * roughness);
	}
	
	ray.direction = reflectDir;
	
	if (reflectivity >= 1.0) {
		ray.weight *= reflectivity;
	} else {
		ray.weight *= hitColor * reflectivity;
	}
	return true;
}

vec3 traceRay(vec3 rayOrigin, vec3 rayDirection, vec3 lightPos) {
	vec3 accumulatedColor = vec3(0.0);
	RayState ray;
	ray.origin = rayOrigin;
	ray.direction = rayDirection;
	ray.weight = vec3(1.0);
	
	for (int bounce = 0; bounce < 6; bounce++) {
		float hitDistance;
		vec3 hitPosition, hitNormal, hitColor;
		float hitRefraction;
		int objectIndex;
		
		if (!intersectScene(ray.origin, ray.direction, hitDistance, hitPosition, hitNormal, hitColor, hitRefraction, objectIndex)){
			accumulatedColor += vec3(0.1, 0.1, 0.2) * ray.weight;
			break;
		}
		
		float reflectivity, transparency;
		getMaterialProperties(hitColor, hitRefraction, reflectivity, transparency);
		
		if (transparency > 0.0) {
			if (!handleTransparentMaterial(ray, hitPosition, hitNormal, hitColor, ray.direction, bounce)) {
				break;
			}
			continue;
		}
		
		if (reflectivity > 0.0) {
			if (!handleReflectiveMaterial(ray, accumulatedColor, hitPosition, hitNormal,hitColor, ray.direction, reflectivity, lightPos, bounce, objectIndex)) {
				break;
			}
			continue;
		}
		vec3 directLighting = calculateDirectLighting(hitColor, hitNormal, hitPosition, lightPos, objectIndex);
		accumulatedColor += directLighting * ray.weight;
		break;
	}
	
	return accumulatedColor;
}

vec3 generateCameraRay(vec2 uv, out vec3 rayOrigin, out vec3 rayDirection) {
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 cameraDir = normalize(cameraTarget - cameraPos);
	vec3 cameraRight = normalize(cross(up, cameraDir));
	vec3 cameraUp = cross(cameraDir, cameraRight);
	
	rayOrigin = cameraPos;
	rayDirection = normalize(cameraDir + uv.x * cameraRight + uv.y * cameraUp);
	
	return vec3(0.0);
}

vec3 renderSample(vec2 offset, vec3 lightPos) {
	vec2 uv = (fragCoord * 0.5 + 0.5) * 2.0 - 1.0;
	uv += offset / iResolution.xy;
	uv.x *= iResolution.x / iResolution.y;
	
	vec3 rayOrigin, rayDirection;
	generateCameraRay(uv, rayOrigin, rayDirection);
	
	return traceRay(rayOrigin, rayDirection, lightPos);
}

vec3 applyToneMapping(vec3 color) {
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(0.4545));
	return color;
}

void main() {
	vec3 lightPos = vec3(sin(iTime) * 5.0, 2.0 + cos(iTime * 0.7) * 2.0, -3.0);
	vec3 finalColor = vec3(0.0);
	int samples = 3;
	
	for (int i = 0; i < samples; i++) {
		for (int j = 0; j < samples; j++) {
			vec2 offset = vec2(float(i), float(j)) / float(samples) - 0.5;
			finalColor += renderSample(offset, lightPos);
		}
	}
	
	finalColor /= float(samples * samples);
	finalColor = applyToneMapping(finalColor);
	
	FragColor = vec4(finalColor, 1.0);
}
