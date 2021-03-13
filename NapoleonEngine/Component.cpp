#include "PCH.h"
#include "Component.h"

using namespace empire;

Component::Component()
	:m_pParentObject{}
{
}

void Component::Init(SceneObject* pParent)
{
	m_pParentObject = pParent;
}