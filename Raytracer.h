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

#pragma region ground_square_sphere
		if(false)
		{
			auto sphere1 = make_shared<Sphere>(vec3(0.0f, 0.0f, 0.6f), 0.4f);
			sphere1->amb = vec3(0.2f, 0.0f, 0.0f);
			sphere1->dif = vec3(1.0f, 0.1f, 0.1f);
			sphere1->spec = vec3(1.5f);
			sphere1->alpha = 50.0f;

			objects.push_back(sphere1);

			auto square1 = make_shared<Square>(vec3(-2.0f, -1.0f, 0.0f), vec3(-2.0f, -1.0f, 4.0f), vec3(2.0f, -1.0f, 4.0f), vec3(2.0f, -1.0f, 0.0f));
			square1->amb = vec3(0.2f);
			square1->dif = vec3(0.8f);
			square1->spec = vec3(1.0f);
			square1->alpha = 50.0f;
			objects.push_back(square1);
		}
#pragma endregion

#pragma region sampling
		if(false)
		{
			auto sphere1 = make_shared<Sphere>(vec3(1.0f, 0.0f, 1.5f), 0.8f);

			sphere1->amb = vec3(0.2f);
			sphere1->dif = vec3(1.0f, 0.2f, 0.2f);
			sphere1->spec = vec3(0.5f);
			sphere1->alpha = 10.0f;

			objects.push_back(sphere1);

//#pragma region sampling_texture
			if(false)
			{
				// 간단한 이미지
				std::vector<vec3> textureImage(4 * 4);
				for (int j = 0; j < 4; j++)
					for (int i = 0; i < 4; i++)
					{
						if (i % 4 == 0)
						{
							textureImage[ i + 4 * j ] = vec3(1.0f, 0.0f, 0.0f) * (1.0f + j) * 0.25f;
						}
						else if (i % 4 == 1)
						{
							textureImage[ i + 4 * j ] = vec3(0.0f, 1.0f, 0.0f) * (1.0f + j) * 0.25f;
						}
						else if (i % 4 == 2)
						{
							textureImage[ i + 4 * j ] = vec3(0.0f, 0.0f, 1.0f) * (1.0f + j) * 0.25f;
						}
						else
						{
							textureImage[ i + 4 * j ] = vec3(1.0f, 1.0f, 1.0f) * (1.0f + j) * 0.25f;
						}
					}

				auto imageTexture = std::make_shared<Texture>(4, 4, textureImage);
			}
//#pragma endregion
			auto imageTexture = std::make_shared<Texture>("shadertoy_abstract1.jpg");

			auto square = make_shared<Square>(vec3(-2.0f, 2.0f, 2.0f), vec3(2.0f, 2.0f, 2.0f), vec3(2.0f, -2.0f, 2.0f), vec3(-2.0f, -2.0f, 2.0f),
											  vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f)); // uv 좌표 (texture 좌표)도 같이 지정

			square->amb = vec3(0.2f);
			square->dif = vec3(1.0f);
			square->spec = vec3(0.0f);

			// square->ambTexture = imageTexture;
			// square->difTexture = imageTexture;

			objects.push_back(square);

			light = Light{ {0.0f, 1.0f, 0.5f } };
		}
#pragma endregion

#pragma region reflection
		if(false)
		{
			auto sphere1 = make_shared<Sphere>(vec3(0.0f, -0.1f, 1.5f), 1.0f);
			sphere1->amb = vec3(0.1f);
			sphere1->dif = vec3(1.0f, 0.0f, 0.0f);
			sphere1->spec = vec3(1.0f);
			sphere1->alpha = 10.0f;
			sphere1->reflection = 0.3f;
			sphere1->transparency = 0.0f;
			objects.push_back(sphere1);

			auto sphere2 = make_shared<Sphere>(vec3(1.2f, -0.7f, 0.5f), 0.4f);
			sphere2->amb = vec3(0.0f);
			sphere2->dif = vec3(0.0f, 0.0f, 1.0f);
			sphere2->spec = vec3(1.0f);
			sphere2->alpha = 50.0f;
			sphere2->reflection = 0.2f;
			objects.push_back(sphere2);

			auto groundTexture = std::make_shared<Texture>("shadertoy_abstract1.jpg");
			auto ground = make_shared<Square>(vec3(-10.0f, -1.2f, 0.0f), vec3(-10.0f, -1.2f, 10.0f), vec3(10.0f, -1.2f, 10.0f), vec3(10.0f, -1.2f, 0.0f),
											  vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f));
			ground->amb = vec4(1.0f);
			ground->dif = vec4(1.0f);
			ground->spec = vec4(1.0f);
			ground->alpha = 10.0f;
			ground->reflection = 0.0f;
			ground->ambTexture = groundTexture;
			ground->difTexture = groundTexture;

			objects.push_back(ground);

			light = Light{ {0.0f, 0.5f, -0.5f} };
		}
#pragma endregion

#pragma region refraction
		{
			auto sphere1 = make_shared<Sphere>(vec3(0.0f, -0.1f, 1.5f), 1.0f);

			sphere1->amb = vec3(0.2f);
			sphere1->dif = vec3(0.0f, 0.0f, 1.0f);
			sphere1->spec = vec3(0.0f);
			sphere1->alpha = 50.0f;
			sphere1->reflection = 0.0f;
			sphere1->transparency = 1.0f;

			objects.push_back(sphere1);

			auto groundTexture = std::make_shared<Texture>("shadertoy_abstract1.jpg");

			auto ground = make_shared<Square>(vec3(-10.0f, -1.5f, 0.0f), vec3(-10.0f, -1.5f, 10.0f), vec3(10.0f, -1.5f, 10.0f), vec3(10.0f, -1.5f, 0.0f),
											  vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f));

			ground->amb = vec3(1.0f);
			ground->dif = vec3(1.0f);
			ground->spec = vec3(1.0f);
			ground->alpha = 10.0f;
			ground->reflection = 0.0f;
			ground->ambTexture = groundTexture;
			ground->difTexture = groundTexture;

			objects.push_back(ground);

			auto squareTexture = std::make_shared<Texture>("back.jpg");
			auto square = make_shared<Square>(vec3(-10.0f, 10.0f, 10.0f), vec3(10.0f, 10.0f, 10.0f), vec3(10.0f, -10.0f, 10.0f), vec3(-10.0f, -10.0f, 10.0f),
											  vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f));

			square->amb = vec3(1.0f);
			square->dif = vec3(0.0f);
			square->spec = vec3(0.0f);
			square->alpha = 10.0f;
			square->reflection = 0.0f;
			square->ambTexture = squareTexture;
			square->difTexture = squareTexture;

			objects.push_back(square);

		}
#pragma endregion

		light = Light{ {0.0f, 0.3f, -0.5f} };
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

				// 텍스춰 좌표
				closestHit.uv = hit.uv;
			}
		}

		return closestHit;
	}
	
	// 광선이 물체에 닿으면 그 물체의 색 반환
	vec3 traceRay(Ray& ray, const int recurseLevel)
	{
		if (recurseLevel < 0)
		{
			return vec3(0.0f);
		}
		// Render first hit
		const auto hit = FindClosestCollision(ray);

		if(hit.d >= 0.0f)
		{
			// 퐁 모델(Phong reflection model)으로 조명 효과 계산
			// 참고 자료
			// https://en.wikipedia.org/wiki/Phong_reflection_model
			// https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF

			glm::vec3 color(0.0f);

			const vec3 dirToLight = glm::normalize(light.pos - hit.point);

			// Shadow (주석처리)
			//Ray shadowRay = { hit.point + dirToLight * 1e-4f, dirToLight }; // add a small vector to avoid numerical issue
			//const auto hit2 = FindClosestCollision(shadowRay);
			//const float dirToLightLen = glm::length(light.pos - hit.point); 
			//if (!(hit2.d >= 0.0f && dirToLightLen > hit2.d))
			{
				glm::vec3 phongColor(0.0f);
				
				// Diffuse
				const float diff = glm::max(dot(hit.normal, dirToLight), 0.0f);

				// Specular
				const vec3 reflectDir = hit.normal * 2.0f * dot(dirToLight, hit.normal) - dirToLight;
				const float specular = glm::pow(glm::max(glm::dot(-ray.dir, reflectDir), 0.0f), hit.obj->alpha);

				// Ambient
				if (hit.obj->ambTexture)
				{
					//phongColor = hit.obj->amb * hit.obj->ambTexture->SamplePoint(hit.uv);
					phongColor += hit.obj->amb * hit.obj->ambTexture->SampleLinear(hit.uv);
				}
				else
				{
					phongColor += hit.obj->amb;
				}

				if (hit.obj->difTexture)
				{
					phongColor += diff * hit.obj->dif * hit.obj->difTexture->SampleLinear(hit.uv);
				}
				else
				{
					phongColor += diff * hit.obj->dif;
				}

				phongColor += hit.obj->spec * specular;

				color += phongColor * (1.0f - hit.obj->reflection - hit.obj->transparency);

				if (hit.obj->reflection)
				{
					// 반사광이 반환해준 색을 더할 때의 비율은 hit.obj->reflection

					const vec3 reflectedDirection = glm::normalize(2.0f * hit.normal *  dot(-ray.dir, hit.normal) + ray.dir);
					Ray reflectRay{ hit.point + reflectedDirection * 1e-4f, reflectedDirection }; // add a small vector to avoid numerical issue

					color += traceRay(reflectRay, recurseLevel - 1) * hit.obj->reflection;
				}

				if (hit.obj->transparency)
				{
					// 투명한 물체의 굴절 처리

					const float ior = 1.5f; // Index of refraction (유리 : 1.5, 물 : 1.3)

					float eta; // sinTheta1 / sinTheta2
					vec3 normal;

					if (glm::dot(ray.dir, hit.normal) < 0.0f) // 밖에서 안으로 들어가는 경우 (예: 공기->유리)
					{
						eta = ior;
						normal = hit.normal;
					}
					else { // 안에서 밖으로 나가는 경우 (예: 유리->공기)
						eta = 1.0f / ior;
						normal = -hit.normal;
					}

					const float cosTheta1 = dot(-ray.dir, normal);
					const float sinTheta1 = sqrt(1.0f - cosTheta1 * cosTheta1); // cos^2 + sin^2 = 1
					const float sinTheta2 = sinTheta1 / eta;
					const float cosTheta2 = sqrt(1.0f - sinTheta2 * sinTheta2);

					const vec3 m = glm::normalize(glm::dot(-ray.dir, normal) * normal + ray.dir);
					const vec3 a = -normal * cosTheta2;
					const vec3 b = m * sinTheta2;
					const vec3 refractedDirection = glm::normalize(a + b);	// transmission
					
					Ray refractedRay{ hit.point + refractedDirection + 1e-4f, refractedDirection };
					color += traceRay(refractedRay, recurseLevel - 1) * hit.obj->transparency;
				}

			}
			//else return hit.obj->amb; // Shadow (주석처리)

			return color;
		}

		return vec3(0.0f);
	}

	/*vec3 traceRay2x2(vec3 eyePos, vec3 pixelPos, const float dx, const int recursiveLevel)
	{
		//:DEBUG:
		// cout << recursiveLevel << " : " << dx << endl;
		if (recursiveLevel == 0)
		{
			Ray myRay{ pixelPos, glm::normalize(pixelPos - eyePos) };
			return traceRay(myRay);
		}

		const float subdx = 0.5f * dx;
		
		vec3 pixelColor(0.0f);
		pixelPos = vec3(pixelPos.x - subdx * 0.5f, pixelPos.y - subdx * 0.5f, pixelPos.z);

		for(int j = 0; j < 2; j++)
			for (int i = 0; i < 2; i++)
			{
				vec3 subPos(pixelPos.x + float(i) * subdx, pixelPos.y + float(j) * subdx, pixelPos.z);
				pixelColor += traceRay2x2(eyePos, subPos, subdx, recursiveLevel - 1);
				//exit(-1); //:DEBUG: 확인용
			}

		return pixelColor * 0.25f;
	}*/

	void Render(std::vector<glm::vec4>& pixels)
	{
		std::fill(pixels.begin(), pixels.end(), vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

		const vec3 eyePos(0.0f, 0.0f, -1.5f);

		const float dx = 2.0f / this->height; // 범위가 (-1, 1)이라서 2를 나눔.

#pragma omp parallel for
		for (int j = 0; j < height; j++)
			for (int i = 0; i < width; i++) {
				const vec3 pixelPosWorld = TransformScreenToWorld(vec2(i, j));

				// const auto rayDir = vec3(0.0f, 0.0f, 1.0f);	// Orthographic projection
				const auto rayDir = glm::normalize(pixelPosWorld - eyePos); // Perspective projection
				Ray pixelRay{ pixelPosWorld, rayDir };
				pixels[ i + width * j ] = vec4(glm::clamp(traceRay(pixelRay, 5), 0.0f, 1.0f), 1.0f);

				//const auto pixelColor = traceRay2x2(eyePos, pixelPosWorld, dx, 3);
				//pixels[ i + width * j ] = vec4(glm::clamp(pixelColor, 0.0f, 1.0f), 1.0f);
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