#include "stb_image.h"
#include "Texture.h"

Texture::Texture(const std::string& filename, const bool bloom)
{
	unsigned char* img = stbi_load(filename.c_str( ), &width, &height, &channels, 0);
	if (width)
	{
		std::cout << filename << " : " << width << " " << height << " " << channels << std::endl;
	}
	else
	{
		std::cout << "Error: reading " << filename << " failed." << std::endl;
		return;
	}

	image.resize(width * height);
	for (int i = 0; i < width * height; i++)
	{
		image[ i ].x = img[ i * channels ] / 255.0f;
		image[ i ].y = img[ i * channels + 1 ] / 255.0f;
		image[ i ].z = img[ i * channels + 2 ] / 255.0f;
		image[ i ].w = 1.0f;
	}

	if (bloom)
	{
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				int idx = i + j * width;

				image[ idx ].x += 1e-4f;
				image[ idx ].y += 1e-4f;
				image[ idx ].z += 1e-4f;
			}
		}

		const float th = 0.3f;

		const std::vector<vec4> pixelsBackup = this->image; // 메모리 내용물까지 모두 복사
		for (int j = 0; j < height; j++)
			for (int i = 0; i < width; i++)
			{
				int idx = i + width * j;
				float lumi = 0.2126 * image[ idx ].x + 0.7152 * image[ idx ].y + 0.0722 * image[ idx ].z;

				if (lumi < th)
				{
					image[ idx ].x = 0.0f;
					image[ idx ].y = 0.0f;
					image[ idx ].z = 0.0f;
				}
			}

		// 밝은 부분만 Blur
		for (int i = 0; i < 300; i++)
		{
			GaussianBlur5( );
		}

		const float weight = 1.0f;

		for (int i = 0; i < pixelsBackup.size( ); i++)
		{
			image[ i ].x = glm::clamp(pixelsBackup[ i ].x + image[ i ].x * weight, 0.0f, 1.0f);
			image[ i ].y = glm::clamp(pixelsBackup[ i ].y + image[ i ].y * weight, 0.0f, 1.0f);
			image[ i ].z = glm::clamp(pixelsBackup[ i ].z + image[ i ].z * weight, 0.0f, 1.0f);
		}
		
	}

	stbi_image_free(img);
}

Texture::Texture(const int& width, const int& height, const std::vector<vec3>& pixels)
	: width(width), height(height), channels(3)
{
	image.resize(width * height * channels);

	for(int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			const auto& color = pixels[ i + j * width ];

			image[ (i + width * j) * channels + 0 ] = vec4(color, 1.0f);
			image[ (i + width * j) * channels + 1 ] = vec4(color, 1.0f);
			image[ (i + width * j) * channels + 2 ] = vec4(color, 1.0f);
		}
}

void Texture::GaussianBlur5( )
{
	std::vector<vec4> pixelsBuffer(this->image.size( ));

	const float weights[ 5 ] = { 0.0545f, 0.2442f, 0.4026f, 0.2442f, 0.0545f };

	// 가로 방향 (x 방향)
#pragma omp parallel for
	for (int j = 0; j < this->height; j++)
	{
		for (int i = 0; i < this->width; i++)
		{
			// 주변 픽셀들의 색을 평균내어서 (i, j)에 있는 픽셀의 색을 변경
			// this->pixels로부터 읽어온 값들을 평균내어서 pixelsBuffer의 값들을 바꾸기

			vec4 avg = vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
			for (int k = 0; k < 5; k++)
			{
				vec3 v = GetClamped(i - 2 + k, j);
				avg.x += v.x * weights[ k ];
				avg.y += v.y * weights[ k ];
				avg.z += v.z * weights[ k ];
			}

			pixelsBuffer[ i + j * this->width ].x = avg.x;
			pixelsBuffer[ i + j * this->width ].y = avg.y;
			pixelsBuffer[ i + j * this->width ].z = avg.z;
			pixelsBuffer[ i + j * this->width ].w = avg.w;
		}
	}

	// Swap
	std::swap(this->image, pixelsBuffer);

	// 세로 방향 (y 방향)
#pragma omp parallel for
	for (int j = 0; j < this->height; j++)
	{
		for (int i = 0; i < this->width; i++)
		{
			// 주변 픽셀들의 색을 평균내어서 (i, j)에 있는 픽셀의 색을 변경
			// this->pixels로부터 읽어온 값들을 평균내어서 pixelsBuffer의 값들을 바꾸기

			vec4 avg = vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
			for (int k = 0; k < 5; k++)
			{
				vec3 v = GetClamped(i, j - 2 + k);
				avg.x += v.x * weights[ k ];
				avg.y += v.y * weights[ k ];
				avg.z += v.z * weights[ k ];
			}

			pixelsBuffer[ i + j * this->width ].x = avg.x;
			pixelsBuffer[ i + j * this->width ].y = avg.y;
			pixelsBuffer[ i + j * this->width ].z = avg.z;
			pixelsBuffer[ i + j * this->width ].w = avg.w;
		}
	}

	// Swap
	std::swap(this->image, pixelsBuffer);
}