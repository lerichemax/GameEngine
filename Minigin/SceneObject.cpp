#include "MiniginPCH.h"
#include "SceneObject.h"

using namespace dae;

SceneObject::SceneObject(Tag tag)
	:m_pComponents{},
	m_IsActive{true},
	m_Tag{tag}
{
	
}

SceneObject::~SceneObject()
{
	for (auto comp : m_pComponents)
	{
		delete comp;
	}
}