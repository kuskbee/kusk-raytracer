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
	shared_ptr<Sphere> sphere;

	Raytracer(const int& width, const int& height)
		: width(width), height(height)
	{
		sphere = make_shared<Sphere>(vec3(0.0f, 0.0f, 0.5f), 0.5f);
		sphere->amb = vec3(0.0f, 0.0f, 0.0f);
		sphere->diff = vec3(0.0f, 0.0f, 1.0f);
		sphere->spec = vec3(1.0f, 1.0f, 1.0f);
		sphere->alpha = 9.0f;
		sphere->ks = 0.8f;

		light = Light{ {0.0f, 0.0f, -1.0f } }; // 화면 뒤쪽
	}
	
	// 광선이 물체에 닿으면 그 물체의 색 반환
	vec3 traceRay(Ray& ray)
	{
		auto hit = sphere->IntersectRayCollision(ray);

		if (hit.d < 0.0f)
		{
			return vec3(0.0f);
		}
		else
		{
			//return sphere->color * hit.d; // 이전 색 결정 버전

			// 퐁 모델(Phong reflection model)으로 조명 효과 계산
			// 참고 자료
			// https://en.wikipedia.org/wiki/Phong_reflection_model
			// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF

			// Diffuse
			const vec3 dirToLight = glm::normalize(light.pos - hit.point);
			const float diff = glm::max(glm::dot(hit.normal, dirToLight), 0.0f);

			// Specular
			const vec3 reflectDir = 2.0f * glm::max(glm::dot(hit.normal, dirToLight), 0.0f) * hit.normal - dirToLight; // 2 (n dot l) n - l
			const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), sphere->alpha);

			return sphere->amb + sphere->diff * diff + sphere->spec * specular * sphere->ks;
			// return sphere->diff * diff;
			// return sphere->spec * specular * sphere->ks;
		}
	}

	void Render(std::vector<glm::vec4>& pixels)
	{
		std::fill(pixels.begin(), pixels.end(), vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

#pragma omp parallel for
		for (int j = 0; j < height; j++)
			for (int i = 0; i < width; i++) {
				const vec3 pixelPosWorld = TransformScreenToWorld(vec2(i, j));

				const auto rayDir = vec3(0.0f, 0.0f, 1.0f);	// Orthographic projection

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