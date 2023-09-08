// Termm--Fall 2020

#pragma once

#include "Material.hpp"

#include <glm/glm.hpp>

#include <list>
#include <string>
#include <iostream>

#include "Common.hpp"

enum class NodeType {
	SceneNode,
	GeometryNode,
	KeyframeNode
};

class SceneNode {
public:
    SceneNode(const std::string & name);

	SceneNode(const SceneNode & other);

    virtual ~SceneNode();
    
	int totalSceneNodes() const;
    
    const glm::mat4& get_transform() const;
    const glm::mat4& get_inverse() const;
    
    void set_transform(const glm::mat4& m);
    
    void add_child(SceneNode* child);
    
    void remove_child(SceneNode* child);

	//-- Transformations:
    virtual void rotate(char axis, float angle);
    virtual void scale(const glm::vec3& amount);
    virtual void translate(const glm::vec3& amount);

    virtual void updateTrans(double time);

    // Raytracing
    virtual bool intersect(const Ray &ray, double tMin, double tMax, HitInfo &hit);

    Ray transformRay(const Ray &ray) const;
    HitInfo transformHitInfo(const HitInfo &hit) const;

    friend std::ostream & operator << (std::ostream & os, const SceneNode & node);

    // Transformations
    glm::mat4 trans;
    glm::mat4 invtrans;
    
    std::list<SceneNode*> children;

	NodeType m_nodeType;
	std::string m_name;
	unsigned int m_nodeId;

private:
	// The number of SceneNode instances.
	static unsigned int nodeInstanceCount;
};
