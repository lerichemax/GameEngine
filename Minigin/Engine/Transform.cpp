#include "MiniginPCH.h"
#include "Transform.h"

using namespace dae;

void Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

Transform& Transform::operator=(Transform const& rhs)
{
	m_Position.x = rhs.m_Position.x;
	m_Position.y = rhs.m_Position.y;
	m_Position.z = rhs.m_Position.z;

	return *this;
}
