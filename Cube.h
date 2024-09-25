#pragma once

#include <vector>

#include "Square.h"

class Cube : public Object
{
public : 
	std::vector<std::shared_ptr<Square>> squares;
	vec2 uv0 = vec2(0.0f);
	vec2 uv1 = vec2(1.0f, 0.0f);
	vec2 uv2 = vec2(1.0f, 1.0f);
	vec2 uv3 = vec2(0.0f, 1.0f);

	Cube(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5, vec3 v6, vec3 v7)
	{
		squares.push_back(std::make_shared<Square>(v0, v1, v2, v3, uv0, uv1, uv2, uv3)); // front
		squares.push_back(std::make_shared<Square>(v4, v0, v3, v7, uv0, uv1, uv2, uv3)); // left
		squares.push_back(std::make_shared<Square>(v1, v5, v6, v2, uv0, uv1, uv2, uv3)); // right
		squares.push_back(std::make_shared<Square>(v5, v4, v7, v6, uv0, uv1, uv2, uv3)); // back
		squares.push_back(std::make_shared<Square>(v4, v5, v1, v0, uv0, uv1, uv2, uv3)); // up
		squares.push_back(std::make_shared<Square>(v3, v2, v6, v7, uv0, uv1, uv2, uv3)); // down

		for (auto iter = squares.begin( ); iter != squares.end( ); iter++)
		{
			(*iter)->amb = vec3(1.0f);
			(*iter)->dif = vec3(0.0f);
			(*iter)->spec = vec3(0.0f);
			(*iter)->alpha = 10.0f;
			(*iter)->reflection = 0.0f;
		}

		/*auto frontTexture = std::make_shared<Texture>("SaintPetersBasilica/posz_blurred.jpg", true);
		auto leftTexture = std::make_shared<Texture>("SaintPetersBasilica/negx_blurred.jpg", true);
		auto rightTexture = std::make_shared<Texture>("SaintPetersBasilica/posx_blurred.jpg", true);
		auto backTexture = std::make_shared<Texture>("SaintPetersBasilica/negz_blurred.jpg", true);
		auto upTexture = std::make_shared<Texture>("SaintPetersBasilica/posy_blurred.jpg", true);
		auto downTexture = std::make_shared<Texture>("SaintPetersBasilica/negy_blurred.jpg", true);*/

		auto frontTexture = std::make_shared<Texture>("SaintPetersBasilica/posz.png");
		auto leftTexture = std::make_shared<Texture>("SaintPetersBasilica/negx.png");
		auto rightTexture = std::make_shared<Texture>("SaintPetersBasilica/posx.png");
		auto backTexture = std::make_shared<Texture>("SaintPetersBasilica/negz.png");
		auto upTexture = std::make_shared<Texture>("SaintPetersBasilica/posy.png");
		auto downTexture = std::make_shared<Texture>("SaintPetersBasilica/negy.png");

		squares[ 0 ]->ambTexture = frontTexture;
		squares[ 0 ]->difTexture = frontTexture;

		squares[ 1 ]->ambTexture = leftTexture;
		squares[ 1 ]->difTexture = leftTexture;
		
		squares[ 2 ]->ambTexture = rightTexture;
		squares[ 2 ]->difTexture = rightTexture;
		
		squares[ 3 ]->ambTexture = backTexture;
		squares[ 3 ]->difTexture = backTexture;
		
		squares[ 4 ]->ambTexture = upTexture;
		squares[ 4 ]->difTexture = upTexture;
		
		squares[ 5 ]->ambTexture = downTexture;
		squares[ 5 ]->difTexture = downTexture;
	}

	virtual Hit CheckRayCollision(Ray& ray) override
	{
		bool isAllMinus = true;
		float minDistance = std::numeric_limits<float>::max( );
		float absDistance = std::numeric_limits<float>::max( );

		Hit result;
		Hit result2;
		for (auto iter = squares.begin( ); iter != squares.end( ); iter++)
		{
			auto hit1 = (*iter)->CheckRayCollision(ray);

			if (hit1.d >= 0.0f)
			{
				isAllMinus = false;
				if (hit1.d < minDistance) {
					minDistance = hit1.d;
					result = hit1;
					result.obj = *iter;
				}
			}
			else
			{
				float abs = glm::abs(hit1.d);
				if (abs < absDistance) {
					absDistance = abs;
					result2 = hit1;
					result2.obj = *iter;
				}
			}
		}

		if (isAllMinus) {
			return result2;
		}

		return result;
	}

};