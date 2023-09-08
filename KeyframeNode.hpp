#pragma once

#include "SceneNode.hpp"

#include <functional>

typedef std::function<double(double)> EasingFunction;


class KeyframeNode : public SceneNode
{
public:
	KeyframeNode(const std::string & name, double startTime, double endTime, uint easing);
	virtual ~KeyframeNode();

	// void setStartTime(double t);
	// void setEndTime(double t);
	// void setEasing(uint index);

	// const glm::mat4& get_transform(double t) const;

	void rotate(char axis, float angle) override;
	void scale(const glm::vec3& amount) override;
	void translate(const glm::vec3& amount) override;

	void updateTrans(double time) override;

private:
	double m_startTime;
	double m_endTime;
	EasingFunction m_easing;

	double m_currentTime;
	
	glm::dvec3 m_translation;
	glm::dvec3 m_rotation;
	glm::dvec3 m_scale;
};
