#pragma once

#include "Sphere.h"
#include "Ray.h"
#include "Light.h"
#include "Triangle.h"
#include "Square.h"
#include "Cube.h"
#include "Camera.h"


#include <vector>

using namespace std;
using namespace glm;

class Raytracer
{
public:
	int width, height;
	Light light;
	std::vector<shared_ptr<Object>> objects;

	// 시점을 결정하는 카메라 클래스 추가
	Camera m_camera;

	bool m_lightRotate = false;
	bool m_enableShadows = false;
	bool m_enableSoftShadows = true;
	bool m_enableReflection = true;
	bool m_enableRefraction = true;
	bool m_enableAntialiasing = false;

	Raytracer(const int& width, const int& height);
	
	Hit FindClosestCollision(Ray& ray);
	
	glm::vec3 RandomOffset( );
	
	// 광선이 물체에 닿으면 그 물체의 색 반환
	vec3 traceRay(Ray& ray, const int recurseLevel);
	vec3 traceRay2x2(vec3 eyePos, vec3 pixelPos, const float dx, const int recursiveLevel);
	
	void UpdateLight(float dt);
	
	void Render(std::vector<glm::vec4>& pixels);
	
	vec3 TransformScreenToWorld(glm::vec2 posScreen);
};