#include "PCH.h"
#include "Component.h"

using namespace empire;

Component::Component()
	:m_pGameObject{}
{
}

void Component::Init(GameObject* pParent)
{
	m_pGameObject = pParent;
}