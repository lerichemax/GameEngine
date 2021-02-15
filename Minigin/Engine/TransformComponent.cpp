#include "MiniginPCH.h"
#include "TransformComponent.h"

using namespace dae;

TransformComponent::TransformComponent(float x, float y, float z)
	:m_Position{x,y,z}
{
}

void TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

TransformComponent& TransformComponent::operator=(TransformComponent const& rhs)
{
	m_Position.x = rhs.m_Position.x;
	m_Position.y = rhs.m_Position.y;
	m_Position.z = rhs.m_Position.z;

	return *this;
}
