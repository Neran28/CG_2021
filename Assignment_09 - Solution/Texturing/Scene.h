#pragma once
#include <memory>
#include <Vec3.h>
#include <vector>
#include "IntersectableObject.h"
#include "LightSource.h"

class Scene
{
	static constexpr float OFFSET_EPSILON = 0.00001f;
	std::vector<std::shared_ptr<const IntersectableObject>> sceneObjects;
	std::vector<std::shared_ptr<const LightSource>> lightSources;
	Vec3 backgroundColor;
	bool debug;

public:
	Scene()
		: Scene(Vec3{0.2f, 0.2f, 0.2f})
	{}

	Scene(const Vec3& backgroundColor)
		: backgroundColor(backgroundColor)
	{ }

	void addObject(std::shared_ptr<const IntersectableObject> object);
	void addLight(std::shared_ptr<const LightSource> ls);
	Vec3 getBackgroundcolor() const;
	std::optional<Intersection> intersect(const Ray& ray, bool shadowRay) const;
	Vec3 traceRay(const Ray& ray, double IOR, int recDepth) const;

	static Scene genTexturedScene();

	void setDebug(bool debug);

};
