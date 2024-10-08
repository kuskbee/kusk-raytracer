#pragma once

#include <iostream>
#include "graphics.h"

class Circle
{
public:
	glm::vec2 center;
	float radius;
	glm::vec4 color;

	Circle(const glm::vec2& center, const float radius, const glm::vec4& color)
		: center(center), color(color), radius(radius)
	{
	}

	bool IsInside(const glm::vec2& x)
	{
		// 원의 방정식 이용
		if (glm::dot(x - center, x - center) - radius * radius > 0.0f)
			return false;
		else
			return true;
	}
};