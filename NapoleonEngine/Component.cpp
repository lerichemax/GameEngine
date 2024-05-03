#include "PCH.h"
#include "Component.h"

Component::Component()
	:m_pGameObject{},
	m_bIsEnabled(false)
{
}

void Component::RootInitialize(GameObject* pParent)
{
	m_pGameObject = pParent;
	m_bIsEnabled = true;
}

ECS_Component::ECS_Component(bool unique)
	:m_IsActive{true},
	m_IsUnique{ unique }
{
}