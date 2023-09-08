// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>

#include "Common.hpp"


bool intersectSphere(
	const glm::dvec4 &centre, const double &radius,
	const Ray &ray, double tMin, double tMax, HitInfo &hit);

bool intersectAABB(
	const glm::dvec3 &minBounds, const glm::dvec3 &maxBounds,
	const Ray &ray, double tMin, double tMax, HitInfo &hit);


class Primitive
{
public:
	virtual ~Primitive() {}

	virtual bool intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit) = 0;
};


class Sphere : public Primitive
{
public:
	virtual ~Sphere() {}

	bool intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit) override;
};


class Cube : public Primitive
{
public:
	virtual ~Cube() {}

	bool intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit) override;
};


class NonhierSphere : public Primitive
{
public:
	NonhierSphere(const glm::vec3 &pos, double radius)
		: m_pos(pos), m_radius(radius)
	{
	}

	virtual ~NonhierSphere() {}

	bool intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit) override;

private:
	glm::vec3 m_pos;
	double m_radius;
};


class NonhierBox : public Primitive
{
public:
	NonhierBox(const glm::vec3 &pos, double size)
		: m_pos(pos), m_size(size)
	{
	}

	virtual ~NonhierBox() {}

	bool intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit) override;

private:
	glm::vec3 m_pos;
	double m_size;
};
