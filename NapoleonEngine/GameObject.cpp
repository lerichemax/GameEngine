#include "PCH.h"
#include "GameObject.h"
//#include "TransformComponent.h"

using namespace empire;

GameObject::GameObject()
	:m_pTransform(new TransformComponent(0.f, 0.f)),
	m_IsActive(true)
{
	AddComponent(m_pTransform);
}

GameObject::~GameObject()
{
	for (auto comp : m_pComponents)
	{
		delete comp;
	}
	m_pComponents.clear();
}

void GameObject::Update(float)
{
	for (auto& c : m_pComponents)
	{
		c->Update();
	}
}

//void GameObject::AddComponent(Component * pComp)
//{
//	if (typeid(*pComp) == typeid(TransformComponent) && HasComponent<TransformComponent>())
//	{
//		std::cout << "Game object already contains a Transform component\n";
//		return;
//	}
//
//	m_pComponents.emplace_back(pComp);
//	pComp->Init(this);
//}