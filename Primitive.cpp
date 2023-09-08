// Termm--Fall 2020

#include "Primitive.hpp"

#include <algorithm>

#include <glm/ext.hpp>

#include "A5.hpp"
#include "Common.hpp"
#include "polyroots.hpp"


bool intersectSphere(
	const glm::dvec4 &center, const double &radius,
	const Ray &ray, double tMin, double tMax, HitInfo &hit)
{
	// ray-sphere algorithm adapted from https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection

	glm::dvec4 L = ray.origin - center;

	double a = glm::dot(ray.direction, ray.direction);
	double b = 2 * glm::dot(ray.direction, L);
	double c = glm::dot(L, L) - radius * radius;

	double roots[2] = {-1, -1};
	size_t numRoots = quadraticRoots(a, b, c, roots);

	// put the roots in order
	auto ts = std::minmax(roots[0], roots[1]);

	// pick the smaller (closer) positive intersection point
	double t = ts.first < 0 ? ts.second : ts.first;

	if (t < std::max(tMin, 0.0) || t > tMax)
		return false;

	hit.t = t;
	hit.pos = ray.origin + t * ray.direction;
	hit.normal = hit.pos - center;
	hit.normal = glm::normalize(hit.normal);

	return true;
}

bool intersectAABB(
	const glm::dvec3 &minBounds, const glm::dvec3 &maxBounds,
	const Ray &ray, double tMin, double tMax, HitInfo &hit)
{
	// (partially) branchless ray-AABB algorithm adapted from https://tavianator.com/2011/ray_box.html

	double tx1 = (minBounds.x - ray.origin.x) / ray.direction.x;
	double tx2 = (maxBounds.x - ray.origin.x) / ray.direction.x;

	double tmin = std::min(tx1, tx2);
	double tmax = std::max(tx1, tx2);

	double ty1 = (minBounds.y - ray.origin.y) / ray.direction.y;
	double ty2 = (maxBounds.y - ray.origin.y) / ray.direction.y;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	double tz1 = (minBounds.z - ray.origin.z) / ray.direction.z;
	double tz2 = (maxBounds.z - ray.origin.z) / ray.direction.z;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));

	if (tmax < tmin)
		return false;

	double t = tmin < 0 ? tmax : tmin;

	if (t < std::max(tMin, 0.0) || t > tMax)
		return false;

	hit.t = t;
	hit.pos = ray.origin + t * ray.direction;

	glm::dvec3 center = (minBounds + maxBounds) * 0.5;
	glm::dvec3 centered = glm::dvec3(hit.pos) - center;
	glm::dvec3 magnitudes = glm::abs(centered) / (maxBounds - minBounds);

	// determine normal
	double maxVal = magnitudes.x;
	int maxDim = 0;
	if (magnitudes.y > maxVal)
	{
		maxVal = magnitudes.y;
		maxDim = 1;
	}
	if (magnitudes.z > maxVal)
	{
		maxDim = 2;
	}
	hit.normal = glm::dvec4(0);
	hit.normal[maxDim] = centered[maxDim] < 0 ? -1 : 1;

	return true;
}


bool Sphere::intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit)
{
	static const glm::dvec4 cOrigin = POINT(0, 0, 0);

	return intersectSphere(cOrigin, 1, ray, tMin, tMax, hit);
}

bool Cube::intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit)
{
	static const glm::dvec3 cMinBounds = glm::dvec3(0);
	static const glm::dvec3 cMaxBounds = glm::dvec3(1);

	return intersectAABB(cMinBounds, cMaxBounds, ray, tMin, tMax, hit);
}

bool NonhierSphere::intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit)
{
	return intersectSphere(POINT(m_pos), m_radius, ray, tMin, tMax, hit);
}

bool NonhierBox::intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit)
{
	glm::dvec3 minBounds = glm::dvec3(m_pos);
	glm::dvec3 maxBounds = minBounds + glm::dvec3(m_size);
	return intersectAABB(minBounds, maxBounds, ray, tMin, tMax, hit);
}
