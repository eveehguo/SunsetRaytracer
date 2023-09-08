// Termm--Fall 2020

#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );

	// Raytracing
	bool intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit) override;

	Material *m_material;
	Primitive *m_primitive;
};
