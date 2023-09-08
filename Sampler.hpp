#pragma once

#include <random>

#include "Common.hpp"

double random01();
double random(double min, double max);

class JitteredSampler2D
{
public:
    JitteredSampler2D(uint gridSize);

    glm::dvec2 next();
    void reset();

    const uint count;

private:
    const uint m_gridSize;
    const double m_subdivSize;
    uint m_current;
};
