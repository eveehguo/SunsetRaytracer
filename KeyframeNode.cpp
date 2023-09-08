#include "KeyframeNode.hpp"

#include "cs488-framework/MathUtils.hpp"


double linear(double x) { return x; }
double easeInQuad(double x) { return x * x; }
double easeOutQuad(double x) { return 1 - (1 - x) * (1 - x); }
double easeInOutQuad(double x) { return x <= 0.5 ? 2 * x * x : 1 - 2 * (1 - x) * (1 - x); }

const EasingFunction easings[]
{
	linear,
	easeInQuad,
	easeOutQuad,
	easeInOutQuad
};


KeyframeNode::KeyframeNode(
	const std::string& name,
	double startTime,
	double endTime,
	uint easing)
	: SceneNode(name)
	, m_startTime(startTime)
	, m_endTime(endTime)
	, m_easing(easings[easing])
	, m_translation(0)
	, m_rotation(0)
	, m_scale(1)
{
	m_nodeType = NodeType::KeyframeNode;
}

KeyframeNode::~KeyframeNode() {
}

// void KeyframeNode::setStartTime(double t)
// {
// 	m_startTime = t;
// }

// void KeyframeNode::setEndTime(double t)
// {
// 	m_endTime = t;
// }

// void KeyframeNode::setEasing(uint index)
// {
// 	m_easing = easings[index];
// }

// const glm::mat4& KeyframeNode::get_transform(double t) const
// {
// 	double s = m_easing(glm::clamp(t, 0.0, 1.0));
	
// 	glm::mat4 trans =
// 		glm::translate(m_translation * s)
// 		* glm::rotate(m_rotation.x * s, glm::dvec3(1, 0, 0))
// 		* glm::rotate(m_rotation.y * s, glm::dvec3(0, 1, 0))
// 		* glm::rotate(m_rotation.z * s, glm::dvec3(0, 0, 1))
// 		* glm::scale(m_scale * s);
	
// 	return trans;
// }

void KeyframeNode::translate(const glm::vec3& amount)
{
	m_translation += amount;
}

void KeyframeNode::rotate(char axis, float angle)
{
	switch (axis) {
		case 'x':
			m_rotation.x += degreesToRadians(angle);
			break;
		case 'y':
			m_rotation.y += degreesToRadians(angle);
	        break;
		case 'z':
			m_rotation.z += degreesToRadians(angle);
	        break;
	}
}

void KeyframeNode::scale(const glm::vec3& amount)
{
	m_scale *= amount;
}

void KeyframeNode::updateTrans(double time)
{
	if (time == m_currentTime)
		return;
	
	m_currentTime = time;
	
	double t = (time - m_startTime) / (m_endTime - m_startTime);
	double s = m_easing(glm::clamp(t, 0.0, 1.0));
	
	trans = glm::translate(s * m_translation)
			* glm::rotate(s * m_rotation.x, glm::dvec3(1.0, 0.0, 0.0))
			* glm::rotate(s * m_rotation.y, glm::dvec3(0.0, 1.0, 0.0))
			* glm::rotate(s * m_rotation.z, glm::dvec3(0.0, 0.0, 1.0))
			* glm::scale(glm::dvec3(1.0) + s * (m_scale - glm::dvec3(1.0)));

	invtrans = glm::inverse(trans);
}
