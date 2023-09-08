// Termm--Fall 2020

#pragma once

#include <iosfwd>

#include <glm/glm.hpp>

#include "Sampler.hpp"

// Represents a simple point light.
class Light
{
public:
	Light();

	virtual uint getNumSamples() { return 1; }
	virtual glm::dvec4 nextPositionSample() { return POINT(position); }
	
	
	glm::vec3 colour;
	glm::vec3 position;
	double falloff[3];
};

class AreaLight : public Light
{
public:
	AreaLight();

	uint getNumSamples() override;
	glm::dvec4 nextPositionSample() override;

	glm::vec3 u;
	glm::vec3 v;

private:
	JitteredSampler2D sampler2D;
};

std::ostream &operator<<(std::ostream &out, const Light &l);
