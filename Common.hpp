#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>


#define POINT(...) glm::dvec4(__VA_ARGS__, 1)
#define VEC(...) glm::dvec4(__VA_ARGS__, 0)


typedef unsigned int uint;


const double DEG2RAD = M_PI / 180.0;
const double EPSILON = 1e-6;
const double SURFACE_BIAS = 1e-4;


// forward declaration
class GeometryNode;

struct Ray
{
    Ray(const glm::dvec4 &origin, const glm::dvec4 &direction, double time, bool opaqueOnly = false)
        : origin(origin)
        , direction(direction)
        , time(time)
        , opaqueOnly(opaqueOnly)
    {};

    glm::dvec4 origin;
    glm::dvec4 direction;
    double time;
    bool opaqueOnly;
};

struct HitInfo
{
    glm::dvec4 pos;
    glm::dvec4 normal;
    double t;
	const GeometryNode *hitObject;
};


template <template <typename, glm::precision> class matType, typename T, glm::precision P>
std::ostream &operator<<(std::ostream &os, const matType<T, P> &v)
{
    return os << glm::to_string(v);
}

template<>
std::ostream &operator<<(std::ostream &os, const glm::mat4 &mat);
