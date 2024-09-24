#pragma once

#include "Object.h"

using namespace glm;

class Sphere : public Object
{
public:
	vec3 center;
	float radius;
	
	Sphere(const vec3 &center, const float radius, const vec3 &color = vec3(1.0f))
		: center(center), radius(radius), Object(color)
	{}

	Hit CheckRayCollision(Ray& ray) override
	{
		Hit hit = Hit{ -1.0f, vec3(0.0f), vec3(0.0f) }; // d가 음수이면 충돌하지 않은 것으로 가정
		
		/*
		 *	hit.d; // 광선의 시작부터 충돌 지점까지의 거리
		 *	hit.point; // 충돌한 위치
		 *	hit.normal; // 충돌한 위치에서 표면의 수직 벡터
		 */

		// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
		// Calculation using vectors in 3D

		// const double a = glm::dot(ray.dir, ray.dir); // dir이 unit vector라면 a는 1.0

		// get nabla
		float b = 2.0f * glm::dot(ray.dir, ray.start - this->center);
		float c = glm::dot(ray.start - this->center, ray.start - this->center) - this->radius * this->radius;
		float det = b * b - 4.0f * c;

		if (det >= 0.0f)
		{
			float dis1 = (- b - sqrt(det)) / 2.0f;
			float dis2 = (- b + sqrt(det)) / 2.0f;

			hit.d = glm::min(dis1, dis2);

			// 물체 안에서 다시 밖으로 나가면서 충돌 가능
			if (hit.d < 0.0f)
				hit.d = glm::max(dis1, dis2);

			hit.point = ray.start + ray.dir * hit.d;
			hit.normal = glm::normalize(hit.point - this->center);
		}

		return hit;
	}

};