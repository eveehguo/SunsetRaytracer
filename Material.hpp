// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>

struct Material {
  Material(
      const glm::vec3 &kd,
      const glm::vec3 &ks,
      double shininess,
      double reflectivity,
      double transparency,
      double refractiveIndex,
      double glossiness);

  glm::vec3 kd;
  glm::vec3 ks;
  double shininess;

  double reflectivity;
  double transparency;
  double refractiveIndex;
  double glossiness; // should really be "unglossiness"
};
