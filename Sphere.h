#pragma once

#include "Hit.h"
#include "Ray.h"

using namespace glm;

class Sphere
{
public:
	glm::vec3 center;
	float radius;
	
	// 퐁 쉐이딩(Phong Shading)을 위한 재질(material)
	vec3 amb = vec3(0.0f);	// Ambient
	vec3 diff = vec3(0.0f); // Diffuse
	vec3 spec = vec3(0.0f);	// Specular
	float ks = 0.0f;
	float alpha = 0.0f;

	//float reflection_ = 0.0;
	//float transparentcy = 0.0f;


	Sphere(const vec3 &center, const float radius)
		: center(center), radius(radius)
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

		// const double a = glm::dot(ray.dir, ray.dir); // dir이 unit vector라면 a는 1.0

		// get nabla
		float b = 2.0f * glm::dot(ray.dir, ray.start - this->center);
		float c = glm::dot(ray.start - this->center, ray.start - this->center) - this->radius * this->radius;
		float det = b * b - 4.0f * c;

		if (det >= 0.0f) {
			float dis1 = (- b - sqrt(det)) / 2.0f;
			float dis2 = (- b + sqrt(det)) / 2.0f;

			hit.d = glm::min(dis1, dis2);
			hit.point = ray.start + ray.dir * hit.d;
			hit.normal = glm::normalize(hit.point - this->center);
		}

		return hit;
	}

};