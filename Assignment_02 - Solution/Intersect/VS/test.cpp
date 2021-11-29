#include <iostream>

struct Point
{
	float x;
	float y;

	Point(float x, float y)
		: x(x), y(y)
	{
		
	}

	Point operator-(const Point& val) const
	{
		return Point{ x - val.x,
					y - val.y,
		};
	}
		static float dot(const Point& a, const Point& b)
		{
			return a.x * b.x + a.y * b.y;
		}
};

void computeBarycentricNew(Point p0, Point p1, Point p2, Point p)
{
	const float det = (p1.x - p0.x) * (p2.y - p0.y) + (p0.x - p2.x) * (p1.y - p0.y);
	
	float a0 = ((p.x - p0.x) * (p2.y - p0.y) + (p0.x - p2.x) * (p.y - p0.y)) / det;
	float a1 = ((p.x - p0.x) * (p0.y - p1.y) + (p1.x - p0.x) * (p.y - p0.y)) / det;
	float a2 = 1 - a0 - a1;

	a0 = int(a0 * 100) / 100.0f;
	a1 = int(a1 * 100) / 100.0f;
	a2 = int(a2 * 100) / 100.0f;

	std::cout << "a0: " << a0 << " a1: " << a1 << " a2: " << a2 << std::endl;
}

void computeBarycentric(Point p0, Point p1, Point p2, Point p)
{
	const float det = (p0.x - p2.x) * (p1.y - p2.y) + (p2.x - p1.x) * (p0.y - p2.y);
	
	float a0 = ((p.x - p2.x) * (p1.y - p2.y) + (p.y - p2.y) * (p2.x - p1.x)) / det;
	float a1 = ((p.x - p2.x) * (p2.y - p0.y) + (p.y - p2.y) * (p0.x - p2.x)) / det;
	float a2 = 1 - a0 - a1;

	a0 = int(a0 * 100) / 100.0f;
	a1 = int(a1 * 100) / 100.0f;
	a2 = int(a2 * 100) / 100.0f;
	
	std::cout << "a0: " << a0 << " a1: " << a1 << " a2: " << a2 << std::endl;
}

void computeBarycentricSE(Point a, Point b, Point c, Point p)
{
	Point v0 = b - a, v1 = c - a, v2 = p - a;
	float d00 = Point::dot(v0, v0);
	float d01 = Point::dot(v0, v1);
	float d11 = Point::dot(v1, v1);
	float d20 = Point::dot(v2, v0);
	float d21 = Point::dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	float a0 = (d11 * d20 - d01 * d21) / denom;
	float a1 = (d00 * d21 - d01 * d20) / denom;
	float a2 = 1.0f - a1 - a0;

	a0 = int(a0 * 100) / 100.0f;
	a1 = int(a1 * 100) / 100.0f;
	a2 = int(a2 * 100) / 100.0f;

	std::cout << "a0: " << a0 << " a1: " << a1 << " a2: " << a2 << std::endl;
}

int main()
{
	const Point Q(2, 1);
	
	Point p0(-3, 4);
	Point p1(5, 2);
	Point p2(1, 0);

	// alle 3 sind gleich
	computeBarycentric(p0, p1, p2, Q);
	computeBarycentricNew(p0, p1, p2, Q);
	computeBarycentricSE(p0, p1, p2, Q);


	return 0;
}