#pragma once

#include "Sphere.h"
#include "Ray.h"
#include "Light.h"
#include "Triangle.h"
#include "Square.h"

#include <vector>

using namespace std;
using namespace glm;

class Raytracer
{
public:
	int width, height;
	Light light;
	shared_ptr<Sphere> sphere;
	std::vector<shared_ptr<Object>> objects;

	Raytracer(const int& width, const int& height)
		: width(width), height(height)
	{
#pragma region sphere3_sample_perspective
		if(false)
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
		}
#pragma endregion

#pragma region square_sphere
		if(false)
		{
			auto sphere1 = make_shared<Sphere>(vec3(0.6f, 0.0f, 0.5f), 0.4f);
			sphere1->amb = vec3(0.1f);
			sphere1->dif = vec3(1.0f, 0.1f, 0.1f);
			sphere1->spec = vec3(1.0f);
			sphere1->alpha = 50.0f;
			objects.push_back(sphere1);

			auto triangle1 = make_shared<Triangle>(vec3(-2.0f, -2.0f, 2.0f), vec3(-2.0f, 2.0f, 5.0f), vec3(2.0f, 2.0f, 5.0f));
			triangle1->amb = vec3(0.2f);
			triangle1->dif = vec3(0.5f);
			triangle1->spec = vec3(0.5f);
			triangle1->alpha = 5.0f;
			objects.push_back(triangle1);

			auto triangle2 = make_shared<Triangle>(vec3(-2.0f, -2.0f, 2.0f), vec3(2.0f, 2.0f, 5.0f), vec3(2.0f, -2.0f, 2.0f));
			triangle2->amb = vec3(0.2f);
			triangle2->dif = vec3(0.5f);
			triangle2->spec = vec3(0.5f);
			triangle2->alpha = 5.0f;
			objects.push_back(triangle2);
		}
#pragma endregion

		auto sphere1 = make_shared<Sphere>(vec3(0.0f, 0.0f, 0.6f), 0.4f);
		sphere1->amb = vec3(0.2f, 0.0f, 0.0f);
		sphere1->dif = vec3(1.0f, 0.1f, 0.1f);
		sphere1->spec = vec3(1.5f);
		sphere1->alpha = 50.0f;

		this->sphere = sphere1; // GUI 연결하기 위해 보관
		objects.push_back(sphere1);

		auto square1 = make_shared<Square>(vec3(-2.0f, -1.0f, 0.0f), vec3(-2.0f, -1.0f, 4.0f), vec3(2.0f, -1.0f, 4.0f), vec3(2.0f, -1.0f, 0.0f));
		square1->amb = vec3(0.2f);
		square1->dif = vec3(0.8f);
		square1->spec = vec3(1.0f);
		square1->alpha = 50.0f;
		objects.push_back(square1);


		light = Light{ {0.0f, 1.0f, 0.2f } };
	}

	Hit FindClosestCollision(Ray& ray)
	{
		Hit closestHit = Hit{ -1.0f, dvec3(0.0), dvec3(0.0) };

		float closestD = numeric_limits<float>::max( );
		for (int l = 0; l < objects.size( ); l++)
		{
			auto hit = objects[ l ]->CheckRayCollision(ray);

			if (hit.d >= 0.0f && hit.d < closestD)
			{
				closestHit = hit;
				closestHit.obj = objects[ l ];
				closestD = hit.d;
			}
		}

		return closestHit;
	}
	
	// 광선이 물체에 닿으면 그 물체의 색 반환
	vec3 traceRay(Ray& ray)
	{
		// Render first hit
		const auto hit = FindClosestCollision(ray);

		if(hit.d >= 0.0f)
		{
			// 퐁 모델(Phong reflection model)으로 조명 효과 계산
			// 참고 자료
			// https://en.wikipedia.org/wiki/Phong_reflection_model
			// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF

			const vec3 dirToLight = glm::normalize(light.pos - hit.point);
			const float dirToLightLen = glm::length(light.pos - hit.point);

			// Shadow
			Ray pointToLight = { hit.point + dirToLight * 1e-4f, dirToLight }; // 충돌점 자체에서 충돌감지되는 것을 방지 (+ dirToLight * 1e-4f)
			const auto hit2 = FindClosestCollision(pointToLight);

			if (hit2.d >= 0.0f && dirToLightLen > hit2.d)
				return hit.obj->amb;
			
			// Diffuse
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