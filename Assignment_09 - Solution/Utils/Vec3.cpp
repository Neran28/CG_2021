#include <iostream>
#include <sstream>
#include <cmath>

#include "Vec3.h"
#include "Rand.h"

Vec3::Vec3() :
	e{0,0,0}
{}

Vec3::Vec3(float x, float y, float z) :  
	e{x,y,z}
{}
	
Vec3::Vec3(const Vec3& other) :
	e{other.e}
{}
	
const std::string Vec3::toString() const {
	std::stringstream s;
	s << "[" << e[0] << ", " << e[1] << ", " << e[2] << "]";
	return s.str();
}

float Vec3::x() const {
	return e[0];
}

float Vec3::y() const {
	return e[1];
}

float Vec3::z() const {
	return e[2];
}

float Vec3::r() const {
	return e[0];
}

float Vec3::g() const {
	return e[1];
}

float Vec3::b() const {
	return e[2];
}

Vec3 Vec3::operator-() const
{
	return Vec3{ -e[0], -e[1], -e[2] };
}

Vec3 Vec3::operator+(const Vec3& val) const{
	return Vec3{e[0]+val.e[0],
				e[1]+val.e[1],
				e[2]+val.e[2]};
}

Vec3 Vec3::operator-(const Vec3& val) const {
	return Vec3{e[0]-val.e[0],
				e[1]-val.e[1],
				e[2]-val.e[2]};
}

Vec3 Vec3::operator*(const Vec3& val) const {
	return Vec3{e[0]*val.e[0],
				e[1]*val.e[1],
				e[2]*val.e[2]};	
}

Vec3 Vec3::operator/(const Vec3& val) const {
	return Vec3{e[0]/val.e[0],
				e[1]/val.e[1],
				e[2]/val.e[2]};
}

Vec3 Vec3::operator*(const float& val) const {
	return Vec3{e[0]*val,
				e[1]*val,
				e[2]*val};
}

Vec3 Vec3::operator/(const float& val) const {
	return Vec3{e[0]/val,
				e[1]/val,
				e[2]/val};
}


bool Vec3::operator == ( const Vec3& other ) const {
	return e == other.e;
}

bool Vec3::operator != ( const Vec3& other ) const {
	return e != other.e;
}

float Vec3::length() const {
	return sqrt(sqlength());
}

float Vec3::sqlength() const {
	return e[0]*e[0]+e[1]*e[1]+e[2]*e[2];
}

float Vec3::dot(const Vec3& a, const Vec3& b) {
	return a.e[0]*b.e[0]+a.e[1]*b.e[1]+a.e[2]*b.e[2];	
}

Vec3 Vec3::cross(const Vec3& a, const Vec3& b) {
	return Vec3{a.e[1] * b.e[2] - a.e[2] * b.e[1],
				a.e[2] * b.e[0] - a.e[0] * b.e[2],
				a.e[0] * b.e[1] - a.e[1] * b.e[0]};
}


Vec3 Vec3::normalize(const Vec3& a) {
  const float l = a.length();
  
  return (l != 0.0f) ? a/l : Vec3{0,0,0};
}

Vec3 Vec3::clamp(const Vec3& val, float minVal, float maxVal)
{
	return Vec3{ std::clamp(val.x(), minVal, maxVal), std::clamp(val.y(), minVal, maxVal), std::clamp(val.z(), minVal, maxVal) };
}

Vec3 Vec3::reflect(const Vec3& a, const Vec3& n) {
	return a-n*dot(a,n)*2;
}

std::optional<Vec3> Vec3::refract(const Vec3& a, const Vec3& normal, const float IOR) {
	float cosI = Vec3::dot(a, normal);
	int sign = (cosI < 0) ? -1 : 1;

	// we assume that if we look from the back side we are exiting the material
	// back side means that the sign/cosI is positive because the incoming ray is assumed to point
	// towards the surface and the normal away
	float n = (sign == 1) ? IOR : 1.0f / IOR;
	float sinThetaSq = n * n * (1.0f - cosI * cosI);

	if (sinThetaSq > 1.0f)
	{
		// Total internal reflection
		return {};
	}
	else
	{
		Vec3 d = a * n;
		float c = n * cosI -sign * sqrtf(1.0f - sinThetaSq);
		Vec3 b = normal * c;
		return d - b;
	}
}

Vec3 Vec3::random() {
	return Vec3{Rand::rand01(),Rand::rand01(),Rand::rand01()};
}

Vec3 Vec3::randomPointInSphere() {
	while (true) {
		Vec3 p{Rand::rand11(),Rand::rand11(),Rand::rand11()};
		if (p.sqlength() > 1) continue;
		return p;
	}
}

Vec3 Vec3::randomPointInHemisphere() {
	while (true) {
		Vec3 p{Rand::rand01(),Rand::rand01(),Rand::rand01()};
		if (p.sqlength() > 1) continue;
		return p;
	}
}

Vec3 Vec3::randomPointInDisc() {
	while (true) {
		Vec3 p{Rand::rand11(),Rand::rand11(),0};
		if (p.sqlength() > 1) continue;
		return p;
	}	
}

Vec3 Vec3::randomUnitVector() {
	const float a = Rand::rand0Pi();
	const float z = Rand::rand11();
	const float r = sqrt(1.0f - z*z);

	return Vec3{r*cosf(a), r*sinf(a), z};
}

Vec3 Vec3::hsvToRgb(const Vec3& other) {
	// Make sure our arguments stay in-range
	const float h = float(int(other.x()) % 360) / 60;
	const float s = std::max(0.0f, std::min(1.0f, other.y()));
	const float v = std::max(0.0f, std::min(1.0f, other.z()));

	if(s == 0) return {v,v,v}; // Achromatic (grey)

	const int i = int(floor(h));
	const float f = h - i; 
	const float p = v * (1 - s);
	const float q = v * (1 - s * f);
	const float t = v * (1 - s * (1 - f));

	switch(i) {
		case 0: return {v,t,p};
		case 1: return {q,v,p};
		case 2: return {p,v,t};
		case 3: return {p,q,v};
		case 4: return {t,p,v};
		default: return {v,p,q};
	}
}

Vec3 Vec3::minV(const Vec3& a, const Vec3& b) {
    return {std::min(a.x(),b.x()), std::min(a.y(),b.y()), std::min(a.z(),b.z())};
}

Vec3 Vec3::maxV(const Vec3& a, const Vec3& b) {
    return {std::max(a.x(),b.x()), std::max(a.y(),b.y()), std::max(a.z(),b.z())};
}

