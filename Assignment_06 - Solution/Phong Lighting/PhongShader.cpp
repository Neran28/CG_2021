#include "PhongShader.h"
#include <cmath>
#include <algorithm>

PhongShader::PhongShader(const Vec3& viewer, const Vec3& light, const Vec3& light_ambient_color, const Vec3& light_diffuse_color, const Vec3& light_specular_color, float exponent)
    : viewer(viewer), light(light), la(light_ambient_color), ld(light_diffuse_color), ls(light_specular_color), r(exponent)
{
}

PhongShader::PhongShader(const PhongShader& other)
    : light(other.light), viewer(other.viewer), la(other.la), ld(other.ld), ls(other.ls), r(other.r)
{
}

Vec3 PhongShader::shade(Vertex surface) const
{
    Material m = surface.material;
    Vec3 c(0, 0, 0);

    c = c + m.color_ambient * la;

    Vec3 N(surface.normal);
    Vec3 L(light - surface.position);
    N = Vec3::normalize(N);
    L = Vec3::normalize(L);

    float d = std::max(0.0f, Vec3::dot(N, L));

    c = c + m.color_diffuse * ld * d;

    float w = 2.0f * Vec3::dot(N, L);
    Vec3 N2 = N * w;

    Vec3 R = N2 - L;
    Vec3 V(viewer - surface.position);
    V = Vec3::normalize(V);

    float s = (float)pow(std::max(0.0f, Vec3::dot(V, R)), r);

    c = c + m.color_specular * ls * s;

    return c;
}
