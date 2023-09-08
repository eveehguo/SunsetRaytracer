// Termm--Fall 2020

#include "Material.hpp"

Material::Material(
      const glm::vec3 &kd,
      const glm::vec3 &ks,
      double shininess,
      double reflectivity,
      double transparency,
      double refractiveIndex,
      double glossiness)
	: kd(kd)
	, ks(ks)
	, shininess(shininess)
	, reflectivity(reflectivity)
	, transparency(transparency)
	, refractiveIndex(refractiveIndex)
	, glossiness(glossiness)
{}
