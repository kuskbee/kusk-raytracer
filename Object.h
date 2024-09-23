#pragma once

#include "graphics.h"

#include "Hit.h"
#include "Ray.h"

using namespace glm;

class Object
{
public:
	// Material
	vec3 amb = vec3(0.0f); // Ambient
	vec3 dif = vec3(0.0f); // Diffuse
	vec3 spec = vec3(0.0f); // Specular

	float alpha = 10.0f;
	// float ks = 1.0f;
	// float reflection_ = 0.0f;
	// float transparency = 0.0f;

	Object(const glm::vec3& color)
		: amb(color), dif(color), spec(color)
	{
	}

	virtual Hit CheckRayCollision(Ray& ray) = 0;
};