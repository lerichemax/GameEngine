#include "MiniginPCH.h"
#include "SceneObject.h"

using namespace dae;

SceneObject::SceneObject()
	:m_pComponents{},
	m_IsActive{true}
{
	
}

SceneObject::~SceneObject()
{
	for (auto comp : m_pComponents)
	{
		delete comp;
	}
}