#pragma once
#include "PhongShader.h"

class BumpPhongShader : public PhongShader
{
private:
	float cellSize;
	float bumpHeight;

public:

	/**
	* @param phong a phong shader to use for the actual shading
	* @param cellSize size of the bumps in the x-y-plane measured in pixels
	* @param bumpHeight amplitude of the bumps in positive z-direction (out of the image plane)
	*/
	BumpPhongShader(const PhongShader& phong, float cellSize, float bumpHeight);

	// Inherited via Shader
	virtual Vec3 shade(Vertex surface) const override;

private:

	float f(float x, float y) const;

	float fx(float x, float y) const;

	float fy(float x, float y) const;

};