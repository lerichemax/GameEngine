#include "MiniginPCH.h"
#include "Component.h"

using namespace dae;

Component::Component()
	:m_pParentObject{}
{
}

void Component::Init(SceneObject* pParent)
{
	m_pParentObject = pParent;
}