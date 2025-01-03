#include "PCH.h"
#include "Component.h"
#include "GameObject.h"

using namespace ecs;

Component::Component()
	:m_IsActive{ true },
	m_IsUnique{ false }
{
}

Component::Component(bool unique)
	:m_IsActive{true},
	m_IsUnique{ unique }
{
}

bool Component::IsActive() const
{
	return m_IsActive;
}

bool Component::IsUnique() const
{
	return m_IsUnique;
}

void Component::SetActive(bool isActive)
{
	m_IsActive = isActive;
}
