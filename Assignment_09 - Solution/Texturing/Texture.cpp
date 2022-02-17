#include "Texture.h"
#include <stb_image.h>
#include <iostream>
#include <cmath>

Texture::Texture(int width, int height)
	: Texture(width, height, FilterMode::BILINEAR)
{
}

Texture::Texture(int width, int height, FilterMode filterMode)
	: Texture(width, height, filterMode, BorderMode::REPEAT)
{
}

Texture::Texture(int width, int height, FilterMode filterMode, BorderMode borderMode)
	: width(width), height(height), filterMode(filterMode), borderModeU(borderMode), borderModeV(borderMode)
{
	borderColor = Vec3{ 0.0f, 0.0f, 0.0f };
	data = std::make_unique<Image>(width, height, 3);
}

Texture::Texture(const std::string& filename)
	: Texture(filename, FilterMode::BILINEAR)
{
}

Texture::Texture(const std::string& filename, FilterMode filterMode)
	: Texture(filename, filterMode, BorderMode::REPEAT)
{
}

Texture::Texture(const std::string& filename, FilterMode filterMode, BorderMode borderMode)
	: filterMode(filterMode), borderModeU(borderMode), borderModeV(borderMode), borderColor(Vec3{0,0,0})
{
	stbi_set_flip_vertically_on_load(false);
	
	int width, height, nrComponents;
	stbi_uc* image_data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if(image_data)
	{
		this->width = width;
		this->height = height;
		this->borderModeU = borderMode;
		this->borderModeV = borderMode;
		this->filterMode = filterMode;

		data = std::make_unique<Image>(width, height, nrComponents, std::vector<uint8_t>{image_data, image_data + (width * height * nrComponents) });
		
		stbi_image_free(image_data);
	}
	else
	{
		std::cerr << "Texture failed to load at path: " << filename << std::endl;
		stbi_image_free(image_data);
	}
}

Texture Texture::genCheckerboardTexture(int width, int height)
{
	Texture checkerboard(width, height, FilterMode::NEAREST, BorderMode::REPEAT);

	float lineValue = 0;
	for (int y = 0; y < height; y++)
	{
		float value = lineValue;
		for (int x = 0; x < width; ++x) {
			checkerboard.data->setNormalizedValue(x, y, 0, value);
			checkerboard.data->setNormalizedValue(x, y, 1, value);
			checkerboard.data->setNormalizedValue(x, y, 2, value);
			value = 1 - value;
		}
		lineValue = 1 - lineValue;
	}

	return checkerboard;
}

// max is already out of range since coordinate indices range from 0 to width/height - 1
int handleBorderCoordinate(int pixelCoord, int max, BorderMode bordermode)
{
	switch (bordermode)
	{
	case BorderMode::CLAMP_TO_EDGE:
		if (pixelCoord < 0)
			pixelCoord = 0;
		if (pixelCoord >= max)
			pixelCoord = max - 1;
		break;
	case BorderMode::REPEAT:
		pixelCoord = pixelCoord % max;
		if (pixelCoord < 0)
			pixelCoord += max;
		break;
	case BorderMode::MIRRORED_REPEAT:
		pixelCoord = pixelCoord % (2 * max);
		if (pixelCoord < 0)
			pixelCoord += max * 2;
		if (pixelCoord >= max)
			pixelCoord = (max * 2 - 1) - pixelCoord;
		break;
	default:
		break;
	}
	return pixelCoord;
}

Vec3 Texture::sample(int pixelCoordX, int pixelCoordY) const
{
	if(borderModeU == BorderMode::CLAMP_TO_BORDER) {
		if (pixelCoordX < 0 || pixelCoordX >= width)
			return borderColor;
	}
	if (borderModeV== BorderMode::CLAMP_TO_BORDER) {
		if (pixelCoordY < 0 || pixelCoordY >= height)
			return borderColor;
	}

	pixelCoordX = handleBorderCoordinate(pixelCoordX, width, borderModeU);
	pixelCoordY = handleBorderCoordinate(pixelCoordY, height, borderModeV);

	Vec3 textureColor{};
	textureColor[0] = data->getValue(pixelCoordX, pixelCoordY, 0) / 255.0f;
	textureColor[1] = data->getValue(pixelCoordX, pixelCoordY, 1) / 255.0f;
	textureColor[2] = data->getValue(pixelCoordX, pixelCoordY, 2) / 255.0f;
	return textureColor;
}

Vec3 Texture::sample(const TextureCoordinates& texCoords) const
{
	const float dx = texCoords.u * width;
	const float dy = texCoords.v * height;

	switch (filterMode)
	{
	case FilterMode::NEAREST:
	{
		const int nx = static_cast<int>(std::floor(dx + 0.5f));
		const int ny = static_cast<int>(std::floor(dy + 0.5f));

		return sample(nx, ny);
	}
	case FilterMode::BILINEAR:
	{
		int fx = (int)floor(dx);
		int fy = (int)floor(dy);
		int cx = (int)ceilf(dx);
		int cy = (int)ceilf(dy);

		Vec3 fxfy = sample(fx, fy);
		Vec3 fxcy = sample(fx, cy);
		Vec3 cxfy = sample(cx, fy);
		Vec3 cxcy = sample(cx, cy);

		float interpX = fabsf(dx - fx);
		float interpY = fabsf(dy - fy);

		Vec3 a = fxfy * (1.0f - interpX) + cxfy * interpX;
		Vec3 b = fxcy * (1.0f - interpX) + cxcy * interpX;
		Vec3 result = a * (1.0f - interpY) + b * interpY;
		return result;
	}

	default:
		std::cout << "Specified bordermode cannot be handled..." << std::endl;
		return Vec3{};
	}
}

void Texture::setBorderMode(BorderMode borderMode)
{
	borderModeU = borderMode;
	borderModeV = borderMode;
}

void Texture::setBorderModeU(BorderMode borderMode)
{
	borderModeU = borderMode;
}

void Texture::setBorderModeV(BorderMode borderMode)
{
	borderModeV = borderMode;
}

void Texture::setBorderColor(const Vec3& borderColor)
{
	this->borderColor = borderColor;
}
