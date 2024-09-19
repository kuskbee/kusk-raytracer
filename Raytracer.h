#pragma once

#include "Sphere.h"
#include "Ray.h"

#include <vector>

using namespace std;
using namespace glm;

class Raytracer
{
public:
	int width, height;
	shared_ptr<Sphere> sphere;

	Raytracer(const int& width, const int& height)
		: width(width), height(height)
	{
		sphere = make_shared<Sphere>(vec3(0.0f, 0.0f, 0.5f), 0.4f, vec3(1.0f, 1.0f, 1.0f));
	}

	glm::vec3 TransformScreenToWorld(glm::vec2 posScreen)
	{
		const float xScale = 2.0f / (this->width - 1);
		const float yScale = 2.0f / (this->height - 1);
		const float aspect = float(this->width) / this->height;

		// 3차원 공간으로 확장 (z좌표는 0.0으로 가정)
		return glm::vec3((posScreen.x * xScale - 1.0f) * aspect, -posScreen.y * yScale + 1.0f, 0.0f);
	}

	// 광선이 물체에 닿으면 그 물체의 색 반환
	vec3 traceRay(Ray& ray)
	{
		const Hit hit = sphere->IntersectRayCollision(ray);

		if (hit.d < 0.0f)
		{
			return vec3(0.0f);
		}
		else
		{
			return sphere->color * hit.d; // 깊이를 곱해서 입체감을 줌.
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

				pixels[size_t(i + width * j)] = vec4(traceRay(pixelRay), 1.0f);
			}
	}
};