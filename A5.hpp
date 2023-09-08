// Term--Spring 2022

#pragma once

#include <glm/glm.hpp>

#include "Atmosphere.hpp"
#include "Common.hpp"
#include "Options.hpp"
#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"

// forward declaration
class Material;

// void A4_Render(
// 	// What to render
// 	SceneNode *root,

// 	// Image to write to, set to a given width and height
// 	Image &image,

// 	// Viewing parameters
// 	const glm::vec3 &eye,
// 	const glm::vec3 &view,
// 	const glm::vec3 &up,
// 	double fovy,

// 	// Lighting parameters
// 	const glm::vec3 &ambient,
// 	const std::list<Light *> &lights);


class A5_Raytracer
{
public:
	A5_Raytracer(
		// Scene to render
		SceneNode *root,

		// Viewing parameters
		const glm::vec3 &eye,
		const glm::vec3 &view,
		const glm::vec3 &up,
		double fovy,

		// Lighting parameters
		const glm::vec3 &ambient,
		std::list<Light *> &lights,

		// DOF
		double focalDistance,
		double blurAmount);

	int render(Image &image, double time=0);

private:
	glm::vec3 getShading(const Ray &ray, int depth);
	glm::vec3 getLightSample(Light *light, const Ray &primaryRay, const Material &material, const HitInfo &hit);
	
	glm::vec3 tonemap(const glm::vec3 &in, float whitePoint=1.f);

	/* PROVIDED DATA */

	// Scene to render
	SceneNode *m_root;

	// Viewing parameters
	const glm::vec3 &m_eye;
	const glm::vec3 &m_view;
	const glm::vec3 &m_up;
	double m_fovy;

	// Lighting parameters
	const glm::vec3 &m_ambient;
	std::list<Light *> &m_lights;

	Light m_sun;

	// DOF
	double m_focalDistance;
	double m_blurAmount;

	Atmosphere atmosphere;

	/* COMPUTED DATA */
	size_t w;
	size_t h;
};
