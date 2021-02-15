#include "MiniginPCH.h"
#include "GameObject.h"

using namespace dae;


GameObject::~GameObject() = default;

void GameObject::Update()
{
	for (auto& c : m_pComponents)
	{
		c->Update();
	}
}