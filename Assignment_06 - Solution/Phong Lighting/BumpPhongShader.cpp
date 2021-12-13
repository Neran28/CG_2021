#define _USE_MATH_DEFINES
#include <cmath>
#include "BumpPhongShader.h"

/**
* @param phong a phong shader to use for the actual shading
* @param cellSize size of the bumps in the x-y-plane measured in pixels
* @param bumpHeight amplitude of the bumps in positive z-direction (out of the image plane)
*/
BumpPhongShader::BumpPhongShader(const PhongShader& phong, float cellSize, float bumpHeight)
	: PhongShader(phong), cellSize(cellSize), bumpHeight(bumpHeight)
{
}

// Inherited via Shader
Vec3 BumpPhongShader::shade(Vertex surface) const
{
	Vec3 cell{ surface.position.x() / cellSize, surface.position.y() / cellSize, 0 };

	Vec3 modifiedPos{ surface.position.x(), surface.position.y() , bumpHeight * f(cell.x(), cell.y()) };
	surface.position = modifiedPos;
	surface.normal = Vec3{ bumpHeight * -fx(cell.x(), cell.y()), bumpHeight * -fy(cell.x(), cell.y()), 1.0f };

	return PhongShader::shade(surface);
}

float BumpPhongShader::f(float x, float y) const
{
	double sinX = sin(M_PI * x);
	double sinY = sin(M_PI * y);
	return sinX * sinX + sinY * sinY;
}

float BumpPhongShader::fx(float x, float y) const
{
	double sinY = sin(M_PI * y);
	return 2 * M_PI * sinY * sinY * cos(M_PI * x) * sin(M_PI * x);
}

float BumpPhongShader::fy(float x, float y) const
{
	double sinX = sin(M_PI * x);
	return 2 * M_PI * sinX * sinX * cos(M_PI * y) * sin(M_PI * y);
}
