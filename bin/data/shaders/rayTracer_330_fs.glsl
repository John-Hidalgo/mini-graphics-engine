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
uniform int numSpheres;
uniform vec3 sphereCenters[10];
uniform float sphereRadii[10];
uniform vec3 sphereColors[10];
uniform int numTriangles;
uniform vec3 triangleV0[10];
uniform vec3 triangleV1[10];
uniform vec3 triangleV2[10];
uniform vec3 triangleColors[10];


bool intersectCube(vec3 rayOrigin, vec3 rayDirection, vec3 cubeCenter, vec3 cubeSize,
				  out float distance, out vec3 position, out vec3 normal, out vec3 color) {
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
		color = vec3(1.0);
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
					out float distance, out vec3 position, out vec3 normal, out vec3 color) {
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
			color = vec3(1.0);
			return true;
		}
	}
	
	return false;
}

bool intersectTriangle(vec3 rayOrigin, vec3 rayDirection, vec3 v0, vec3 v1, vec3 v2,
					  out float distance, out vec3 position, out vec3 normal, out vec3 color) {
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
		color = vec3(1.0);
		return true;
	}
	
	return false;
}

bool intersectFloor(vec3 rayOrigin, vec3 rayDirection,
				   out float distance, out vec3 position, out vec3 normal, out vec3 color) {
	if (rayDirection.y < 0.0) {
		float t = (-2.0 - rayOrigin.y) / rayDirection.y;
		if (t > 0.0) {
			distance = t;
			position = rayOrigin + rayDirection * t;
			normal = vec3(0.0, 1.0, 0.0);
			color = vec3(0.7);
			return true;
		}
	}
	return false;
}

bool intersectScene(vec3 rayOrigin, vec3 rayDirection,out float distance, out vec3 position, out vec3 normal, out vec3 color) {
	float closestDist = 1e10;
	vec3 closestPos, closestNormal, closestColor;
	bool foundHit = false;
	float floorDist;
	vec3 floorPos, floorNormal, floorColor;
	if (intersectFloor(rayOrigin, rayDirection, floorDist, floorPos, floorNormal, floorColor)) {
		if (floorDist < closestDist) {
			closestDist = floorDist;
			closestPos = floorPos;
			closestNormal = floorNormal;
			closestColor = floorColor;
			foundHit = true;
		}
	}
	for (int i = 0; i < numCubes; i++) {
		float dist;
		vec3 pos, norm, col;
		if (intersectCube(rayOrigin, rayDirection, cubeCenters[i], cubeSizes[i], dist, pos, norm, col)) {
			if (dist < closestDist) {
				closestDist = dist;
				closestPos = pos;
				closestNormal = norm;
				closestColor = cubeColors[i];
				foundHit = true;
			}
		}
	}
	for (int i = 0; i < numSpheres; i++) {
		float dist;
		vec3 pos, norm, col;
		if (intersectSphere(rayOrigin, rayDirection, sphereCenters[i], sphereRadii[i], dist, pos, norm, col)) {
			if (dist < closestDist) {
				closestDist = dist;
				closestPos = pos;
				closestNormal = norm;
				closestColor = sphereColors[i];
				foundHit = true;
			}
		}
	}
	for (int i = 0; i < numTriangles; i++) {
		float dist;
		vec3 pos, norm, col;
		if (intersectTriangle(rayOrigin, rayDirection, triangleV0[i], triangleV1[i], triangleV2[i], dist, pos, norm, col)) {
			if (dist < closestDist) {
				closestDist = dist;
				closestPos = pos;
				closestNormal = norm;
				closestColor = triangleColors[i];
				foundHit = true;
			}
		}
	}
	
	if (foundHit) {
		distance = closestDist;
		position = closestPos;
		normal = closestNormal;
		color = closestColor;
		return true;
	}
	
	return false;
}

float calculateShadow(vec3 pos, vec3 lightPos) {
	vec3 lightDir = normalize(lightPos - pos);
	float shadowDist;
	vec3 shadowPos, shadowNormal, shadowColor;
	
	if (intersectScene(pos + lightDir * 0.01, lightDir, shadowDist, shadowPos, shadowNormal, shadowColor)) {
		float distToLight = length(lightPos - pos);
		if (shadowDist < distToLight) {
			return 0.3;
		}
	}
	
	return 1.0;
}

float getReflectivity(vec3 color) {
	float brightness = (color.r + color.g + color.b) / 3.0;
	if (color == vec3(1.0, 0.2, 0.2)) {
		return 0.0;
	} else if (color == vec3(0.2, 1.0, 0.2)) {
		return 1.0;
	} else if (color == vec3(0.7, 0.7, 0.7)) {
		return 1.0;
	} else if (color == vec3(0.2, 0.2, 1.0)) {
		return 0.0;
	}
	
	return brightness * 0.6;
}

void main() {
	vec3 finalColor = vec3(0.0);

	int samples = 3;
	
	for (int i = 0; i < samples; i++) {
		for (int j = 0; j < samples; j++) {
			vec2 offset = vec2(float(i), float(j)) / float(samples) - 0.5;
			vec2 uv = (fragCoord * 0.5 + 0.5) * 2.0 - 1.0;
			uv += offset / iResolution.xy;
			uv.x *= iResolution.x / iResolution.y;
			
			vec3 up = vec3(0.0, 1.0, 0.0);
			vec3 cameraDir = normalize(cameraTarget - cameraPos);
			vec3 cameraRight = normalize(cross(up, cameraDir));
			vec3 cameraUp = cross(cameraDir, cameraRight);
			
			vec3 rayOrigin = cameraPos;
			vec3 rayDirection = normalize(cameraDir + uv.x * cameraRight + uv.y * cameraUp);
			
			vec3 lightPos = vec3(sin(iTime) * 5.0, 2.0 + cos(iTime * 0.7) * 2.0, -3.0);
			vec3 accumulatedColor = vec3(0.0);
			vec3 rayWeight = vec3(1.0);
			for (int bounce = 0; bounce < 4; bounce++) {
				float hitDistance;
				vec3 hitPosition, hitNormal, hitColor;
				
				if (intersectScene(rayOrigin, rayDirection, hitDistance, hitPosition, hitNormal, hitColor)) {
					vec3 lightDir = normalize(lightPos - hitPosition);
					float diffuse = max(dot(hitNormal, lightDir), 0.0);
					float shadow = calculateShadow(hitPosition, lightPos);
					vec3 ambient = vec3(0.1);
					vec3 directLighting = hitColor * (ambient + diffuse * shadow);
					accumulatedColor += directLighting * rayWeight;
					float reflectivity = getReflectivity(hitColor);
					if (reflectivity > 0.0 && bounce < 3) {
						rayOrigin = hitPosition + hitNormal * 0.01;
						rayDirection = reflect(rayDirection, hitNormal);
						rayWeight *= reflectivity;
					}
					else {
						break;
					}
				}
				else {
					accumulatedColor += vec3(0.1, 0.1, 0.2) * rayWeight;
					break;
				}
			}
			
			finalColor += accumulatedColor;
		}
	}
	finalColor /= float(samples * samples);
	finalColor = finalColor / (finalColor + vec3(1.0));
	finalColor = pow(finalColor, vec3(0.4545));
	FragColor = vec4(finalColor, 1.0);
}
