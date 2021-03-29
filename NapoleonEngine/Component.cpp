#include "PCH.h"
#include "Component.h"

using namespace empire;

Component::Component()
	:m_pParentObject{}
{
}

void Component::Init(GameObject* pParent)
{
	m_pParentObject = pParent;
}