#pragma once

#include "graphics.h"
#include <string>
#include <vector>

// Bilinear interpolation reference
// https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/interpolation/bilinear-filtering

using namespace glm;

class Texture
{	
public:
	int width, height, channels;
	std::vector<uint8_t> image;

	Texture(const std::string& filename);
	Texture(const int& width, const int& height, const std::vector<vec3>& pixels);
	
	vec3 GetClamped(int i, int j)
	{
		i = glm::clamp(i, 0, width - 1);
		j = glm::clamp(j, 0, height - 1);

		const float r = image[ (i + width * j) * channels + 0 ] / 255.0f;
		const float g = image[ (i + width * j) * channels + 1 ] / 255.0f;
		const float b = image[ (i + width * j) * channels + 2 ] / 255.0f;

		return vec3(r, g, b);
	}

	vec3 GetWrapped(int i, int j)
	{
		i %= width;
		j %= height;

		if (i < 0)
			i += width; // i가 -1이면 (width - 1)로 바뀜
		if (j < 0)
			j += height;

		const float r = image[ (i + width * j) * channels + 0 ] / 255.0f;
		const float g = image[ (i + width * j) * channels + 1 ] / 255.0f;
		const float b = image[ (i + width * j) * channels + 2 ] / 255.0f;

		return vec3(r, g, b);
	}

	vec3 InterpolateBilinear(
		const float &dx,
		const float &dy,
		const vec3 &c00,
		const vec3 &c10,
		const vec3 &c01,
		const vec3 &c11
	)
	{
		vec3 ret(0.0f);
		const vec3 horizon1 = c00 * (1 - dx) + c10 * dx;
		const vec3 horizon2 = c01 * (1 - dx) + c11 * dx;
		const vec3 vertical = horizon1 * (1 - dy) + horizon2 * dy;

		return vertical;
	}

	vec3 SamplePoint(const vec2& uv) // Nearest sampling
	{
		// 텍스춰 좌표의 범위 [0.0, 1.0] x [0.0, 1.0]
		// 이미지 좌표의 범위 [-0.5, width - 1 + 0.5] x [-0.5, height - 1 + 0.5]
		// 배열 인덱스의 정수 범위 ij [0, width - 1] x [0, height -1]

		vec2 xy = uv * vec2(float(width), float(height)) - vec2(0.5f);

		int i = glm::round(xy.x);
		int j = glm::round(xy.y);

		return GetClamped(i, j);
	}

	vec3 SampleLinear(const vec2& uv)
	{
		// 텍스춰 좌표의 범위 [0.0, 1.0] x [0.0, 1.0]
		// 이미지 좌표의 범위 [-0.5, width - 1 + 0.5] x [-0.5, height - 1 + 0.5]
		// std::cout << floor(-0.3f) << " " << int(-0.3f) << std::endl; // -1 0

		vec2 xy = uv * vec2(float(width), float(height)) - vec2(0.5f);
		const int i = int(glm::floor(xy.x));
		const int j = int(glm::floor(xy.y));
		const float dx = xy.x - float(i);
		const float dy = xy.y - float(j);

		return InterpolateBilinear(dx, dy, GetClamped(i, j), GetClamped(i + 1, j), GetClamped(i, j + 1), GetClamped(i + 1, j + 1));
		//return InterpolateBilinear(dx, dy, GetWrapped(i, j), GetWrapped(i + 1, j), GetWrapped(i, j + 1), GetWrapped(i + 1, j + 1));
	}
};

