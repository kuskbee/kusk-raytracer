#pragma once

#include "graphics.h"

#include "Hit.h"
#include "Ray.h"
#include "Texture.h"

using namespace glm;

class Object
{
public:
	// Material
	vec3 amb = vec3(0.0f); // Ambient
	vec3 dif = vec3(0.0f); // Diffuse
	vec3 spec = vec3(0.0f); // Specular

	float alpha = 10.0f;
	
	float reflection = 0.0f;
	float transparency = 0.0f;

	std::shared_ptr<Texture> ambTexture;
	std::shared_ptr<Texture> difTexture;

	Object(const vec3& color = {1.0f, 1.0f, 1.0f})
		: amb(color), dif(color), spec(color)
	{
	}

	virtual Hit CheckRayCollision(Ray& ray) = 0;
};