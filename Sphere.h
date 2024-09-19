#pragma once

#include "Hit.h"
#include "Ray.h"

using namespace glm;

class Sphere
{
public:
	glm::vec3 center;
	float radius;
	glm::vec3 color;

	Sphere(const glm::vec3 &center, const float radius, const glm::vec3 &color)
		: center(center), color(color), radius(radius)
	{}

	Hit IntersectRayCollision(Ray& ray)
	{
		Hit hit = Hit{ -1.0f, vec3(0.0f), vec3(0.0f) }; // d가 음수이면 충돌하지 않은 것으로 가정
		
		/*
		 *	hit.d; // 광선의 시작부터 충돌 지점까지의 거리
		 *	hit.point; // 충돌한 위치
		 *	hit.normal; // 충돌한 위치에서 표면의 수직 벡터
		 */

		// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
		// Calculation using vectors in 3D

		// get nabla
		float first = glm::dot(ray.dir, ray.start - center);
		float second = glm::dot(ray.start - center, ray.start - center);
		float nabla = first * first - (second - radius * radius);

		if (nabla >= 0) {
			float dis1 = -first - nabla;
			float dis2 = -first + nabla;

			if (dis1 > 0 || dis2 > 0) {
				hit.d = (dis1 < dis2) ? dis1 : dis2;
				hit.point = ray.start + hit.d * ray.dir;
				hit.normal = glm::normalize(hit.point - center);
			}

		}

		return hit;
	}

};