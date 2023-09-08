// Termm--Fall 2020

#include <iostream>

#include <glm/ext.hpp>

#include "Light.hpp"
#include "Options.hpp"

Light::Light()
	: colour(0.0, 0.0, 0.0),
	  position(0.0, 0.0, 0.0)
{
	falloff[0] = 1.0;
	falloff[1] = 0.0;
	falloff[2] = 0.0;
}

AreaLight::AreaLight()
	: Light(), u(0), v(0),
	sampler2D(AREA_LIGHT_SAMPLER_GRID)
{
}

uint AreaLight::getNumSamples()
{
	sampler2D.reset();
	return sampler2D.count;
}

glm::dvec4 AreaLight::nextPositionSample()
{
	glm::dvec2 xi = sampler2D.next();
	return POINT(position + u * xi.x + v * xi.y);
}


std::ostream &operator<<(std::ostream &out, const Light &l)
{
	out << "L[" << glm::to_string(l.colour)
		<< ", " << glm::to_string(l.position) << ", ";
	for (int i = 0; i < 3; i++)
	{
		if (i > 0)
			out << ", ";
		out << l.falloff[i];
	}
	out << "]";
	return out;
}
