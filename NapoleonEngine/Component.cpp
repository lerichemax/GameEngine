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