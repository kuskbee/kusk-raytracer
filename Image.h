#pragma once

#include <vector>

#include "graphics.h"

class Image
{
public:
	int width = 0, height = 0, channels = 0;
	std::vector<Vec4> pixels; // �̹��� ó���� ���� ���� float�� �����ϴ� ���� �� ����

	void ReadFromFile(const char* filename);
	void WritePNG(const char* filename);
	Vec4& GetPixel(int i, int j);
	void BoxBlur5();
	void GaussianBlur5();
	void Bloom(const float& th, const int& numRepeat, const float& weight = 1.0f);
};
