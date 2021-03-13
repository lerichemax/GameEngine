#include "PCH.h"
#include "SceneObject.h"

using namespace empire;

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