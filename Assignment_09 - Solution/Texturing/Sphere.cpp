#include "Sphere.h"
#include <Mat3.h>


#ifndef M_PI
constexpr float M_PI = 3.14159265358979323846f;
#endif

Sphere::Sphere(const Vec3& center, double radius, const Material& material)
	: Sphere(center, radius, material, Vec3{ 0, 0, 0 }, TextureCoordinates{ 1.0f, 1.0f }, TextureCoordinates{0.0f, 0.0f})
{

}

Sphere::Sphere(const Vec3& center, double radius, const Material& material, const Vec3& rotation)
	: Sphere(center, radius, material, rotation, TextureCoordinates{ 1.0f, 1.0f }, TextureCoordinates{ 0.0f, 0.0f })
{

}

Sphere::Sphere(const Vec3& center, double radius, const Material& material, const Vec3& rotation, const TextureCoordinates& scale, const TextureCoordinates& bias)
	: center(center), sqradius(radius*radius), material(material), rotation(rotation), scale(scale), bias(bias)
{

}

Material Sphere::getMaterial() const
{
	return material;
}

std::optional<Intersection> Sphere::intersect(const Ray& ray) const
{
	Vec3 l = center - ray.getOrigin();

	double tCenter = Vec3::dot(l, ray.getDirection());
	if (tCenter < 0)
		return {};	// no intersection

	double dSq = l.sqlength() - tCenter * tCenter;
	if (dSq > sqradius)
		return {};	// no intersection

	double dist = sqrt(sqradius - dSq);
	double t = tCenter - dist;

	if (t < 0)
		t = tCenter + dist;	// when inside sphere

	Vec3 normal = ray.getPosOnRay(t) - center;
	normal = Vec3::normalize(normal);

	std::optional<TextureCoordinates> tc{};
	if (material.hasTexture())
	{
		// rotate coordinate frame
		Vec3 r =  Vec3(normal);
		r = Mat3::rotationX(rotation.x()) * r;
		r = Mat3::rotationY(rotation.y()) * r;
		r = Mat3::rotationZ(rotation.z()) * r;

		// normalize between [0, 1]
		float u = atan2f(r.y(), r.x()) * 0.5f / M_PI + 0.5f;
		float v = acos(r.z() / r.length()) / M_PI;
		tc = TextureCoordinates{ u, v };

		// re-scale and bias
		tc->u = tc->u * scale.u + bias.u;
		tc->v = tc->v * scale.v + bias.v;
	}

	return Intersection{ material, normal, tc, t };
}
