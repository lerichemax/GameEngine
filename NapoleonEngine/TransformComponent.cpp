#include "PCH.h"
#include "TransformComponent.h"

using namespace empire;

TransformComponent::TransformComponent()
	:m_Position(),
	m_Scale(1, 1, 1)
{
	
}


TransformComponent::TransformComponent(float x, float y, float z)
	:m_Position{x,y,z},
	m_Scale(1,1,1)
{
}

void TransformComponent::Translate(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void TransformComponent::Translate(glm::vec3 const& pos)
{
	m_Position = pos;
}

void TransformComponent::Translate(glm::vec2 const& pos)
{
	m_Position.x = pos.x;
	m_Position.y = pos.y;
}

void TransformComponent::Scale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void TransformComponent::Scale(glm::vec3 const& scale)
{
	m_Scale.x = scale.x;
	m_Scale.y = scale.y;
	m_Scale.z = scale.z;
}

void TransformComponent::Scale(float uniformScale)
{
	m_Scale.x = uniformScale;
	m_Scale.y = uniformScale;
	m_Scale.z = uniformScale;
}

TransformComponent& TransformComponent::operator=(TransformComponent const& rhs)
{
	m_Position.x = rhs.m_Position.x;
	m_Position.y = rhs.m_Position.y;
	m_Position.z = rhs.m_Position.z;

	return *this;
}

void TransformComponent::Rotate(float rot)
{
	m_Rotation = rot;
}