#pragma once

#include <glm/glm.hpp>

#include "Common.hpp"

// constants taken from https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/simulating-sky

const double earthRadius = 6.36e6;
const double atmosphereRadius = 6.42e6;
const double rayleighH = 7994;
const double mieH = 1200;

const glm::vec3 rayleighBeta(3.8e-6, 13.5e-6, 33.1e-6);
const glm::vec3 mieBeta(21e-6);


class Atmosphere
{
public:
    Atmosphere(
        glm::dvec4 sunDirection = VEC(0, 1, 0),
        double sunIntensity = 20)
        // glm::dvec3 groundKd = glm::dvec3(0.3, 0.7, 0.3))
        : sunDirection(sunDirection)
        , sunIntensity(sunIntensity)
        // , groundKd(groundKd)
    {}

    static double rayleighPhase(double mu);
    static double miePhase(double mu);

    glm::vec3 getAttenuation(const Ray &ray, double tMax = INFINITY) const;
    // glm::vec3 getSky(const Ray &ray) const;

    glm::dvec4 sunDirection;
    double sunIntensity;
    // glm::dvec3 groundKd;
};
