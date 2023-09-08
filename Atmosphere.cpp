#include "Atmosphere.hpp"

#include "Primitive.hpp"

#include "Options.hpp"

double Atmosphere::rayleighPhase(double mu)
{
    return 3.0 / (16.0 * M_PI) * (1 + mu * mu);
}

double Atmosphere::miePhase(double mu)
{
    static const double g = 0.76;
    static const double gSquared = g * g;

    return 3.0 / (8.0 * M_PI) * ((1.0 - gSquared) * (1.0 + mu * mu))
            / ((2.0 + gSquared) * pow(1.0 + gSquared - 2.0 * g * mu, 1.5));
}


glm::vec3 Atmosphere::getAttenuation(const Ray &ray, double tMax) const
{
    // adapted from https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/simulating-sky

    glm::dvec4 origin = ray.origin + VEC(0, earthRadius, 0);

    Ray primaryRay(origin, ray.direction, ray.time);
    HitInfo hit{};
    bool hitFound = intersectSphere(POINT(0, 0, 0), atmosphereRadius, primaryRay, 0, INFINITY, hit);
    if (!hitFound)
        return glm::vec3(0);

    double segmentLength = std::min(tMax, hit.t) / ATMOSPHERE_PRIMARY_SAMPLES;
    double tPrimary = segmentLength * 0.5;

    glm::vec3 rayleigh;
    glm::vec3 mie;

    double opticalDepthR = 0;
    double opticalDepthM = 0;
    for (uint i = 0; i < ATMOSPHERE_PRIMARY_SAMPLES; ++i)
    {
        glm::dvec4 samplePosition = origin + tPrimary * ray.direction;
        double height = glm::length(samplePosition) - earthRadius;

        // compute optical depth for light
        double hr = exp(-height / rayleighH) * segmentLength;
        double hm = exp(-height / mieH) * segmentLength;
        opticalDepthR += hr;
        opticalDepthM += hm;

        // light optical depth
        Ray lightRay(samplePosition, sunDirection, ray.time);
        HitInfo lightHit{};
        intersectSphere(POINT(0, 0, 0), atmosphereRadius, lightRay, 0, INFINITY, lightHit);
        
        double segmentLengthLight = lightHit.t / ATMOSPHERE_LIGHT_SAMPLES;
        double tLight = 0;
        double opticalDepthLightR = 0;
        double opticalDepthLightM = 0;
        uint j;
        for (j = 0; j < ATMOSPHERE_LIGHT_SAMPLES; ++j)
        {
            glm::dvec4 samplePositionLight = samplePosition + (tLight + segmentLengthLight * 0.5) * sunDirection;
            double heightLight = glm::length(samplePositionLight) - earthRadius;
            if (heightLight < 0)
                break;

            opticalDepthLightR += exp(-heightLight / rayleighH) * segmentLengthLight;
            opticalDepthLightM += exp(-heightLight / mieH) * segmentLengthLight;
            tLight += segmentLengthLight;
        }
        if (j == ATMOSPHERE_LIGHT_SAMPLES)
        {
            glm::vec3 tau = rayleighBeta * (opticalDepthR + opticalDepthLightR) + mieBeta * 1.1 * (opticalDepthM + opticalDepthLightM);
            glm::vec3 attenuation = glm::exp(-tau);
            rayleigh += attenuation * hr;
            mie += attenuation * hm;
        }
        tPrimary += segmentLength;
    }

    double mu = glm::dot(ray.direction, sunDirection);

    return (rayleigh * rayleighBeta * rayleighPhase(mu) + mie * mieBeta * miePhase(mu)) * sunIntensity;
}

// glm::vec3 Atmosphere::getSky(const Ray &ray) const
// {
//     glm::vec3 attenuation = getAttenuation(ray, INFINITY);
//     if (attenuation == glm::vec3(0))
//         return groundKd * sunDirection.y;
//     return attenuation;
// }