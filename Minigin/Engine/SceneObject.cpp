#include "MiniginPCH.h"
#include "SceneObject.h"

using namespace dae;

SceneObject::SceneObject()
	:m_Transform{},
	m_pComponents{}
{
}

void SceneObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}