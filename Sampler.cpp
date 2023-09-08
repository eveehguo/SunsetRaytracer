#include "Sampler.hpp"

#include <random>

#include <glm/ext.hpp>

double random01()
{
    static std::mt19937 gen;
    static std::uniform_real_distribution<double> distr;
    
    return distr(gen);
}

double random(double min, double max)
{
    return random01() * (max - min) + min;
}

JitteredSampler2D::JitteredSampler2D(uint gridSize)
    : m_gridSize(gridSize),
    m_subdivSize(1.0 / gridSize),
    count(gridSize * gridSize),
    m_current(0)
{
}

glm::dvec2 JitteredSampler2D::next()
{
    uint x = m_current % m_gridSize;
    uint y = m_current / m_gridSize;

    m_current++;

    m_current = m_current % count;

    return glm::dvec2(
        m_subdivSize * (x + random01()),
        m_subdivSize * (y + random01())
    );
}

void JitteredSampler2D::reset()
{
    m_current = 0;
}
