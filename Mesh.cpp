// Termm--Fall 2020

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "cs488-framework/Exception.hpp"

#include "Options.hpp"
#include "Mesh.hpp"


Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	std::ifstream ifs( fname.c_str(), std::ios::in );
    ifs.exceptions(std::ifstream::badbit);

    if (!ifs) {
        std::stringstream errorMessage;
        errorMessage << "Unable to open .obj file " << fname.c_str()
            << " within method ObjFileDecoder::decode" << std::endl;
        throw Exception(errorMessage.str());
    }

    std::string line;
    int vertex1, vertex2, vertex3;
    int normal1, normal2, normal3;

	while( !ifs.eof() ) {
		// if (code == "v")
		// {
		// 	ifs >> vx >> vy >> vz;
		// 	m_vertices.push_back(glm::vec3(vx, vy, vz));
		// }
		// else if (code == "f")
		// {
		// 	ifs >> s1 >> s2 >> s3;
		// 	m_faces.push_back(Face(s1 - 1, s2 - 1, s3 - 1));
		// }
        try {
            std::getline(ifs, line);
        } catch (const std::ifstream::failure &e) {
            ifs.close();
            std::stringstream errorMessage;
            errorMessage << "Error calling getline() : " << e.what() << std::endl;
            throw Exception(errorMessage.str());
        }
		
		if (line.substr(0, 2) == "v ") {
            // Vertex data on this line.
            // Get entire line excluding first 2 chars.
            std::istringstream s(line.substr(2));
            glm::vec3 vertex;
            s >> vertex.x;
            s >> vertex.y;
            s >> vertex.z;
            m_vertices.push_back(vertex);

        } else if (line.substr(0, 3) == "vn ") {
            // Normal data on this line.
            // Get entire line excluding first 2 chars.
            std::istringstream s(line.substr(2));
            glm::vec3 normal;
            s >> normal.x;
            s >> normal.y;
            s >> normal.z;
            m_normals.push_back(normal);

        } else if (line.substr(0, 2) == "f ") {
            // Face index data on this line.

            int index;

            // sscanf will return the number of matched index values it found
            // from the pattern.
            int numMatches = sscanf(line.c_str(), "f %d %d %d",
                                              &vertex1, &vertex2, &vertex3);

            if (numMatches == 3) {
				normal1 = 0; // will be adjusted to -1
				normal2 = 0;
				normal3 = 0;
			} else {
                // Line contains indices of the pattern vertex//normal.
                numMatches = sscanf(line.c_str(), "f %d//%d %d//%d %d//%d",
		               &vertex1, &normal1,
                       &vertex2, &normal2,
                       &vertex3, &normal3);
				
				if (numMatches != 6)
				{
					ifs.close();
					std::stringstream errorMessage;
					errorMessage << "Error parsing file: " << fname.c_str() 
							<< " -- line: " << line.c_str() << std::endl;
					throw Exception(errorMessage.str());
				}
            }

			m_faces.emplace_back(vertex1 - 1, vertex2 - 1, vertex3 - 1,
					normal1 - 1, normal2 - 1, normal3 - 1);
        }
	}

	// construct bounding box
	m_minBounds = glm::vec3(INFINITY);
	m_maxBounds = glm::vec3(-INFINITY);
	for (const glm::vec3 &vert : m_vertices)
	{
		m_minBounds.x = std::min(vert.x, m_minBounds.x);
		m_minBounds.y = std::min(vert.y, m_minBounds.y);
		m_minBounds.z = std::min(vert.z, m_minBounds.z);

		m_maxBounds.x = std::max(vert.x, m_maxBounds.x);
		m_maxBounds.y = std::max(vert.y, m_maxBounds.y);
		m_maxBounds.z = std::max(vert.z, m_maxBounds.z);
	}
}

bool Mesh::intersectFace(
	const Face &face,
	const Ray &ray, double tMin, double tMax, HitInfo &hit)
{
	// cast to doubles for precision
	glm::dvec3 dir = glm::dvec3(ray.direction);
	glm::dvec3 orig = glm::dvec3(ray.origin);
	glm::dvec3 dv0 = glm::dvec3(m_vertices[face.v0]);
	glm::dvec3 dv1 = glm::dvec3(m_vertices[face.v1]);
	glm::dvec3 dv2 = glm::dvec3(m_vertices[face.v2]);

	// Moller-Trumbore algorithm from https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection

	glm::dvec3 v0v1 = dv1 - dv0;
	glm::dvec3 v0v2 = dv2 - dv0;
	glm::dvec3 pvec = glm::cross(dir, v0v2);
	double det = glm::dot(v0v1, pvec);

	if (det < EPSILON)
		return false;

	double invDet = 1.0 / det;

	glm::dvec3 tvec = orig - dv0;
	double u = glm::dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1)
		return false;

	glm::dvec3 qvec = glm::cross(tvec, v0v1);
	double v = glm::dot(dir, qvec) * invDet;
	if (v < 0 || u + v > 1)
		return false;

	double t = glm::dot(v0v2, qvec) * invDet;
	if (t < std::max(tMin, 0.0) || t > tMax)
		return false;

	hit.t = t;
	hit.pos = ray.origin + hit.t * ray.direction;

	if (face.n0 == -1 || face.n1 == -1 || face.n2 == -1)
	{
		hit.normal = VEC(glm::normalize(glm::cross(v0v1, v0v2)));
	}
	else
	{
		glm::dvec3 dn0 = m_normals[face.n0];
		glm::dvec3 dn1 = m_normals[face.n1];
		glm::dvec3 dn2 = m_normals[face.n2];
		hit.normal = VEC(glm::normalize((1 - u - v) * dn0 + u * dn1 + v * dn2));
	}

	return true;
}

bool Mesh::intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit)
{
#ifdef RENDER_BOUNDING_VOLUMES
	// add epsilon to prevent issues with meshes with degenerate bounds like the floor plane
	return intersectAABB(m_minBounds, m_maxBounds + glm::vec3(EPSILON), ray, tMin, tMax, hit);
#else
	HitInfo boundsHit{};
	if (!intersectAABB(m_minBounds, m_maxBounds, ray, tMin, tMax, boundsHit))
		return false;

    bool hitFound = false;
	for (const Face &face : m_faces)
	{
		if (intersectFace(face, ray, tMin, tMax, hit))
		{
			hitFound = true;
			tMax = hit.t;
		}
	}
    return hitFound;
#endif
}


std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}
