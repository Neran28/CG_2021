#pragma once
#include "Shader.h"

class PhongShader : public Shader
{
private:
	Vec3 light, viewer;
	Vec3 la, ld, ls;
	float r;

public:

	PhongShader(const Vec3& viewer, const Vec3& light, const Vec3& light_ambient_color, const Vec3& light_diffuse_color, const Vec3& light_specular_color, float exponent);

	PhongShader(const PhongShader& other);

	virtual Vec3 shade(Vertex surface) const override;
};