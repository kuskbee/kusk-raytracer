#pragma once

#include "Object.h"

using namespace glm;

class Triangle : public Object
{
public :
	vec3 v0, v1, v2;
	// vec2 uv0, uv1, uv2; // 텍스춰 좌표계에서 사용

public :
	Triangle( )
		: v0(vec3(0.0f)), v1(vec3(0.0f)), v2(vec3(0.0f))
	{
	}

	Triangle(vec3 v0, vec3 v1, vec3 v2)
		: v0(v0), v1(v1), v2(v2)
	{
	}

	virtual Hit CheckRayCollision(Ray& ray) override
	{
		Hit hit = Hit{ -1.0f, vec3(0.0), vec3(0.0) };
		vec3 point, faceNormal;
		float t, u, v;
		if (IntersectRayTriangle(ray.start, ray.dir, v0, v1, v2,
								 point, faceNormal, t, u, v))
		{
			hit.d = t;
			hit.point = point;
			hit.normal = faceNormal;

			// 텍스춰링(texturing)에서 사용
			// hit.uv = uv0 * u + uv1 * v + uv2 * (1.0f - u - v);
		}

		return hit;
	}

	// 참고 :
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
	bool IntersectRayTriangle(const vec3& orig, const vec3& dir,
							  const vec3& v0, const vec3& v1, const vec3& v2,
							  vec3& point, vec3& faceNormal,
							  float& t, float& u, float& v)
	{
		/*
		 * 기본 전략
		 * - 삼각형이 놓여있는 평면과 광선의 교점을 찾고,
		 * - 그 교점이 삼각형 안에 있는지 밖에 있는지를 판단한다.
		 */

		 // 주의 : 삼각형의 넓이가 0일 경우에는 계산할 수 없음.
		 // (v1 - v0 이나 v2 - v0가 0 벡터이면 그럴 수 있음.)
		if (glm::dot(v1 - v0, v1 - v0) < 1e-2f || glm::dot(v2 - v0, v2 - v0) < 1e-2f) return false;

		/*1. 삼각형이 놓여 있는 평면의 수직 벡터 계산*/
		faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
		
		// 삼각형 뒷면을 그리고 싶지 않은 경우 (Backface culling)
		if (glm::dot(-dir, faceNormal) < 0.0f) return false;

		// 평면과 광선이 수평에 매우 가깝다면 충돌하지 못하는 것으로 판단
		if (glm::abs(glm::dot(dir, faceNormal)) < 1e-2f) return false; // t 계산 시 0으로 나누기 방지

		/*2. 광선과 평면의 충돌 위치 계산*/
		t = (glm::dot(v0, faceNormal) - glm::dot(orig, faceNormal)) / glm::dot(dir, faceNormal);

		// 광선의 시작점 이전에 충돌한다면 렌더링할 필요 없음.
		if (t < 0.0f) return false;

		point = orig + t * dir; // 충돌점

		/*3. 그 충돌 위치가 삼각형 안에 들어 있는 지 확인*/

		// 작은 삼각형들 3개의 normal 계산
		// 방향만 확인하면 되기 때문에 normalize 생략 가능.
		const vec3 normal0 = glm::cross(point - v2, v1 - v2);
		const vec3 normal1 = glm::cross(v0 - v2, point - v2);
		const vec3 normal2 = glm::cross(v1 - v0, point - v0);
		
		if (dot(normal0, faceNormal) < 0.0f) return false;
		if (dot(normal1, faceNormal) < 0.0f) return false;
		if (dot(normal2, faceNormal) < 0.0f) return false;
		
		// Barycentric coordinate 계산
		// 텍스춰링(texturing)에서 사용
		// 아래에서 cross poduct의 절대값으로 작은 삼각형 넓이 계산
		
		// u = ...
		// v = ...

		return true;
	}

};