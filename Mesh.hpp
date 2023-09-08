// Termm--Fall 2020

#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Common.hpp"
#include "Primitive.hpp"

// Use this #define to selectively compile your code to render the
// bounding boxes around your mesh objects. Uncomment this option
// to turn it on.
//#define RENDER_BOUNDING_VOLUMES

struct Face
{
	int v0;
	int v1;
	int v2;

	int n0;
	int n1;
	int n2;

	Face(int pv0, int pv1, int pv2,
		int pn0, int pn1, int pn2)
		: v0(pv0), v1(pv1), v2(pv2),
		  n0(pn0), n1(pn1), n2(pn2)
	{
	}
};

// A polygonal mesh.
class Mesh : public Primitive
{
public:
	Mesh(const std::string &fname);

	bool intersectFace(const Face &face, const Ray &ray, double tMin, double tMax, HitInfo &hit);
	bool intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit) override;

private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<Face> m_faces;

	glm::vec3 m_minBounds;
	glm::vec3 m_maxBounds;

	friend std::ostream &operator<<(std::ostream &out, const Mesh &mesh);
};
