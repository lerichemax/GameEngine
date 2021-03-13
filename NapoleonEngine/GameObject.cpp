#include "PCH.h"
#include "GameObject.h"

#include "TransformComponent.h"

using namespace empire;

GameObject::GameObject()
{
	AddComponent<TransformComponent>(0.f,0.f);
}

GameObject::~GameObject() = default;

void GameObject::Update(float)
{
	for (auto& c : m_pComponents)
	{
		c->Update();
	}
}