// Term--Spring 2022

#include "A5.hpp"

#include <chrono>

#include <glm/ext.hpp>

#include "Common.hpp"
#include "GeometryNode.hpp"
// #include "PhongMaterial.hpp"
#include "Sampler.hpp"

A5_Raytracer::A5_Raytracer(
	SceneNode *root,
	const glm::vec3 &eye,
	const glm::vec3 &view,
	const glm::vec3 &up,
	double fovy,
	const glm::vec3 &ambient,
	std::list<Light *> &lights,
	double focalDistance,
	double blurAmount
)
	: m_root(root),
	  m_eye(eye),
	  m_view(view),
	  m_up(up),
	  m_fovy(fovy),
	  m_ambient(ambient),
	  m_lights(lights),
	  m_focalDistance(focalDistance),
	  m_blurAmount(blurAmount),
	  atmosphere()
{
	m_sun.colour = glm::vec3(1.0);
	m_lights.push_back(&m_sun);
}

int A5_Raytracer::render(Image &image, double time)
{

	// std::cout << "Calling A4_Render(\n"
	// 		  << "\t" << *m_root << "\t"
	// 		  << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
	// 		  << "\t" << "eye:  " << glm::to_string(m_eye) << std::endl
	// 		  << "\t" << "view: " << glm::to_string(m_view) << std::endl
	// 		  << "\t" << "up:   " << glm::to_string(m_up) << std::endl
	// 		  << "\t" << "fovy: " << m_fovy << std::endl
	// 		  << "\t" << "ambient: " << glm::to_string(m_ambient) << std::endl
	// 		  << "\t" << "lights{" << std::endl;

	// for (const Light *light : m_lights)
	// {
	// 	std::cout << "\t\t" << *light << std::endl;
	// }
	// std::cout << "\t}" << std::endl;
	// std::cout << ")" << std::endl;

	// time execution

	auto startClock = std::chrono::high_resolution_clock::now();
	
	double sunTheta = time / 24.0 * 2*M_PI;
	glm::dvec4 sunDir = VEC(0, -cos(sunTheta), sin(sunTheta));
	atmosphere.sunDirection = sunDir;
	m_sun.position = glm::vec3(atmosphere.sunDirection) * SUN_LIGHT_DISTANCE;
	m_sun.colour = atmosphere.getAttenuation(Ray(POINT(0, m_eye.y, 0), sunDir, time));

	h = image.height();
	w = image.width();

	// for now we assume the eye is at the origin and we are looking towards -z
	// then we will transform the scene to account for eye position and view direction
	double yTopLeft = tan(m_fovy * DEG2RAD / 2.0);
	double xTopLeft = -yTopLeft * (double)w / h;

	double pixelWidth = -2.0 * xTopLeft / w;
	double pixelHeight = 2.0 * yTopLeft / h;

	glm::dvec4 topLeft = VEC(xTopLeft, yTopLeft, -1.0);

	double lensSize = m_blurAmount * m_focalDistance;
#ifndef ENABLE_AA
	// disable AA -- adjust to centre of pixel
	topLeft += glm::dvec4(pixelWidth / 2.0, -pixelHeight / 2.0, 0, 0);
	JitteredSampler2D primarySampler(DOF_SAMPLER_GRID);
#else
	JitteredSampler2D primarySampler(AA_SAMPLER_GRID);
#endif

	// scale to image plane for DOF
	topLeft *= m_focalDistance;
	pixelWidth *= m_focalDistance;
	pixelHeight *= m_focalDistance;

	// turn topLeft into a "point"
	// topLeft[3] = 1;

	glm::mat4 viewTransform = glm::lookAt(m_eye, m_eye + m_view, m_up);
	viewTransform[3] = glm::vec4(0, 0, 0, 1);
	
	// glm::dvec4 viewUp = VEC(m_up);
	// glm::dvec4 viewRight = VEC(glm::cross(m_view, m_up));

	glm::dvec4 eyePoint = POINT(m_eye);

	for (uint y = 0; y < h; ++y)
	{
		for (uint x = 0; x < w; ++x)
		{
			// std::cout << x << "," << y;
			
			glm::vec3 shading;

			primarySampler.reset();
			for (uint i = 0; i < primarySampler.count; i++)
			{
				glm::dvec2 sample = primarySampler.next();

#ifndef ENABLE_AA
				glm::dvec4 pixelVec = topLeft + VEC(x * pixelWidth, y * -pixelHeight, 0.0);
				glm::dvec4 origin = lensSize * VEC((sample.x * 2.0 - 1.0), (sample.y * 2.0 - 1.0), 0.0);
#else
				glm::dvec4 pixelVec = topLeft + VEC((x + sample.x) * pixelWidth, (y + sample.y) * -pixelHeight, 0.0);
				glm::dvec4 origin = lensSize * VEC(random(-1.0, 1.0), random(-1.0, 1.0), 0.0);
#endif
				glm::dvec4 dir = pixelVec - origin;

				origin = origin * viewTransform;
				origin += eyePoint;
				dir = dir * viewTransform;
				dir = glm::normalize(dir);
				
				// std::cout << "d:" << dir << " o:" << origin << std::endl;
				
				Ray ray(origin, dir, time);
				shading += getShading(ray, MAX_DEPTH);
			}

			shading /= primarySampler.count;

			// std::cout << std::endl;

			shading = tonemap(shading, 1.4f);

			image(x, y, 0) = shading.r;	
			image(x, y, 1) = shading.g;
			image(x, y, 2) = shading.b;
		}
	}

	auto endClock = std::chrono::high_resolution_clock::now();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(endClock - startClock).count();
	return millis;
}

// compute fresnel factor using the Schlick approximation
double fresnel(double cosTheta, double eta)
{
	double numer = (eta - 1.0)*(eta - 1.0);
	double denom = (eta + 1.0)*(eta + 1.0);
	double R0 = numer / denom;
    return R0 + (1.0 - R0) * pow(1.0 - cosTheta, 5.0);
}

glm::dvec4 perturbeSphere(const glm::dvec4 &direction, double radius)
{
	// generate a point in the unit sphere
	glm::dvec4 p;
	double invLength;
	do {
		double x = random(-1.0, 1.0);
		double y = random(-1.0, 1.0);
		double z = random(-1.0, 1.0);
		p = VEC(x, y, z);
		invLength = glm::inversesqrt(glm::dot(p, p));
	} while (invLength < 1.0);

	p *= invLength; // normalize to get a point on the unit sphere
	p *= radius;

	return glm::normalize(direction + p);
}

glm::vec3 A5_Raytracer::getShading(const Ray &ray, int depth)
{
	HitInfo hit{};
	bool hitFound = m_root->intersect(ray, 0, INFINITY, hit);

	glm::vec3 shading;
	if (hitFound)
	{
		hit.normal = glm::normalize(hit.normal);

		Material *material = hit.hitObject->m_material;

		// atmospheric fog
		shading += atmosphere.getAttenuation(ray, hit.t);

		double localContribution = (1.0 - material->reflectivity) * (1.0 - material->transparency);

		if (localContribution > 0)
		{
			// ambient lighting
			glm::vec3 localShading = material->kd * m_ambient;

			// direct lighting
			for (Light *light : m_lights)
			{
				glm::vec3 lightShading;
				uint numSamples = light->getNumSamples();

				for (uint i = 0; i < numSamples; i++)
				{
					lightShading += getLightSample(light, ray, *material, hit);
				}
				localShading += lightShading / numSamples;
			}
			shading += localContribution * localShading;
		}

		// compute recursive reflection & refracton
		if (depth > 0)
		{
			double cosTheta = -glm::dot(ray.direction, hit.normal);
			double eta = 1 / material->refractiveIndex;
			glm::dvec4 N = hit.normal;

			if (cosTheta < 0)
			{
				cosTheta = -cosTheta;
				eta = material->refractiveIndex;
				N = -N;
			}

			double fresnelReflectance = fresnel(cosTheta, eta);

			double reflectedContribution = material->reflectivity * (1.0 - material->transparency) + material->transparency * fresnelReflectance;
			double refractedContribution = material->transparency * (1.0 - fresnelReflectance);

			// compute reflection
			if (reflectedContribution > 0)
			{
				glm::dvec4 reflectDir = ray.direction + 2 * cosTheta * N;
				glm::dvec4 reflectOrigin = hit.pos + N * SURFACE_BIAS;

				if (material->glossiness == 0)
				{
					Ray reflectRay(reflectOrigin, reflectDir, ray.time);
					shading += reflectedContribution * getShading(reflectRay, depth - 1);
				}
				else
				{
					glm::vec3 glossyShading;
					for (int i = 0; i < GLOSSY_SAMPLES; i++)
					{
						Ray reflectRay(reflectOrigin, perturbeSphere(reflectDir, material->glossiness), ray.time);
						glossyShading += getShading(reflectRay, depth - 1);
					}
					shading += reflectedContribution * (glossyShading / GLOSSY_SAMPLES);
				}
			}

			// compute refraction
			if (refractedContribution > 0)
			{
				double k = 1 - eta * eta * (1 - cosTheta * cosTheta);
				// (k < 0) means total internal reflection
				if (k > 0)
				{
					glm::dvec4 refractDir = eta * ray.direction + (eta * cosTheta - sqrt(k)) * N;
					glm::dvec4 refractOrigin = hit.pos - N * SURFACE_BIAS;
					// std::cout << " r" << hit.pos;
					refractDir = glm::normalize(refractDir);
					
					if (material->glossiness == 0)
					{
						Ray refractRay(refractOrigin, refractDir, ray.time);
						shading += refractedContribution * getShading(refractRay, depth - 1);
					}
					else
					{
						glm::vec3 glossyShading;
						for (int i = 0; i < GLOSSY_SAMPLES; i++)
						{
							Ray refractRay(refractOrigin, perturbeSphere(refractDir, material->glossiness), ray.time);
							glossyShading += getShading(refractRay, depth - 1);
						}
						shading += refractedContribution * (glossyShading / GLOSSY_SAMPLES);
					}
				}
			}
		}
	}
	else
	{
		shading = atmosphere.getAttenuation(ray);
	}
	return shading;
}

glm::vec3 A5_Raytracer::getLightSample(Light *light, const Ray &primaryRay, const Material &material, const HitInfo &hit)
{
	glm::dvec4 shadowRayOrigin = hit.pos + hit.normal * SURFACE_BIAS;

	glm::dvec4 pos = light->nextPositionSample();
	glm::dvec4 l = pos - hit.pos;
	double dist = glm::length(l);
	l /= dist; // normalize l

	Ray shadowRay(shadowRayOrigin, l, primaryRay.time, true);
	HitInfo shadowHit{};
	bool shadow = m_root->intersect(shadowRay, 0, dist, shadowHit);

	double shadowStrength = 1.0;
	if (shadow)
	{
		return glm::vec3(0);
	}

	glm::dvec4 v = glm::normalize(primaryRay.origin - hit.pos);
	double l_dot_n = glm::dot(l, hit.normal);

#ifndef BLINN_PHONG
	glm::dvec4 r = -l + 2 * l_dot_n * hit.normal;
	glm::vec3 diffuse = material.kd * l_dot_n;
	glm::vec3 specular = material.ks * std::pow(glm::dot(r, v), material.shininess);
#else
	glm::dvec4 h = glm::normalize(v + l);
	glm::vec3 diffuse = material.kd * l_dot_n;
	glm::vec3 specular = material.ks * std::pow(glm::dot(h, hit.normal), material.shininess);
#endif

	double attenuation = 1.0 / (light->falloff[0] + light->falloff[1] * dist + light->falloff[2] * dist*dist);

	return light->colour * (diffuse + specular) * attenuation * shadowStrength;
}

float luminance(const glm::vec3 &in)
{
    return glm::dot(in, glm::vec3(0.2126f, 0.7152f, 0.0722f));
}

glm::vec3 A5_Raytracer::tonemap(const glm::vec3 &in, float whitePoint)
{
	float l_in = luminance(in);
	float l_out = l_in * (1.0f + (l_in / (whitePoint * whitePoint)))
				/ (1.0f + l_in);
	return in * (l_out / l_in);
}
