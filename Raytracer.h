#pragma once

#include "Sphere.h"
#include "Ray.h"
#include "Light.h"

#include <vector>

using namespace std;
using namespace glm;

class Raytracer
{
public:
	int width, height;
	Light light;
	std::vector<shared_ptr<Object>> objects;

	Raytracer(const int& width, const int& height)
		: width(width), height(height)
	{
		// 스크린으로부터 거리가 다른 구 3개
		auto sphere1 = make_shared<Sphere>(vec3(0.5f, 0.0f, 0.5f), 0.4f, vec3(0.5f, 0.5f, 0.5f));
		auto sphere2 = make_shared<Sphere>(vec3(0.0f, 0.0f, 1.0f), 0.4f, vec3(0.5f, 0.5f, 0.5f));
		auto sphere3 = make_shared<Sphere>(vec3(-0.5f, 0.0f, 1.5f), 0.4f, vec3(0.5f, 0.5f, 0.5f));

		sphere1->amb = vec3(0.2f);
		sphere1->dif = vec3(1.0f, 0.2f, 0.2f);
		sphere1->spec = vec3(0.5f);
		sphere1->alpha = 10.0f;

		sphere2->amb = vec3(0.2f);
		sphere2->dif = vec3(0.2f, 1.0f, 0.2f);
		sphere2->spec = vec3(0.5f);
		sphere2->alpha = 10.0f;

		sphere3->amb = vec3(0.2f);
		sphere3->dif = vec3(0.2f, 0.2f, 1.0f);
		sphere3->spec = vec3(0.5f);
		sphere3->alpha = 10.0f;

		// 일부러 역순으로 추가
		objects.push_back(sphere3);
		objects.push_back(sphere2);
		objects.push_back(sphere1);

		light = Light{ {0.0f, 1.0f, -1.0f } }; // 화면 뒤쪽
	}

	Hit FindClosestCollision(Ray& ray)
	{
		Hit closest_hit = Hit{ -1.0f, dvec3(0.0), dvec3(0.0) };

		float smallest = numeric_limits<float>::max( );
		for (int l = 0; l < objects.size( ); l++)
		{
			auto hit = objects[ l ]->CheckRayCollision(ray);

			if (hit.d >= 0.0f && hit.d < smallest)
			{
				closest_hit = hit;
				closest_hit.obj = objects[ l ];
				smallest = hit.d;
			}
		}

		return closest_hit;
	}
	
	// 광선이 물체에 닿으면 그 물체의 색 반환
	vec3 traceRay(Ray& ray)
	{
		// Render first hit
		const auto hit = FindClosestCollision(ray);

		if(hit.d >= 0.0f)
		{
			//return sphere->color * hit.d; // 이전 색 결정 버전

			// 퐁 모델(Phong reflection model)으로 조명 효과 계산
			// 참고 자료
			// https://en.wikipedia.org/wiki/Phong_reflection_model
			// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF

			// Diffuse
			const vec3 dirToLight = glm::normalize(light.pos - hit.point);
			const float diff = glm::max(dot(hit.normal, dirToLight), 0.0f);

			// Specular
			const vec3 reflectDir = 2.0f * glm::max(dot(hit.normal, dirToLight), 0.0f) * hit.normal - dirToLight; // 2 (n dot l) n - l
			const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.obj->alpha);

			return hit.obj->amb + hit.obj->dif * diff + hit.obj->spec * specular;
			// return sphere->diff * diff;
			// return sphere->spec * specular;
		}

		return vec3(0.0f);
	}

	void Render(std::vector<glm::vec4>& pixels)
	{
		std::fill(pixels.begin(), pixels.end(), vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

		const vec3 eyePos(0.0f, 0.0f, -1.5f);

#pragma omp parallel for
		for (int j = 0; j < height; j++)
			for (int i = 0; i < width; i++) {
				const vec3 pixelPosWorld = TransformScreenToWorld(vec2(i, j));

				//const auto rayDir = vec3(0.0f, 0.0f, 1.0f);	// Orthographic projection
				const auto rayDir = glm::normalize(pixelPosWorld - eyePos); // Perspective projection
				Ray pixelRay{ pixelPosWorld, rayDir };

				pixels[ size_t(i + width * j) ] = vec4(glm::clamp(traceRay(pixelRay), 0.0f, 1.0f), 1.0f);
			}
	}

	vec3 TransformScreenToWorld(glm::vec2 posScreen)
	{
		const float xScale = 2.0f / this->width;
		const float yScale = 2.0f / this->height;
		const float aspect = float(this->width) / this->height;

		// 3차원 공간으로 확장 (z좌표는 0.0으로 가정)
		return glm::vec3((posScreen.x * xScale - 1.0f) * aspect, -posScreen.y * yScale + 1.0f, 0.0f);
	}
};