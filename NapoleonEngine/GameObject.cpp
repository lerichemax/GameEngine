#include "PCH.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "TextRendererComponent.h"
using namespace empire;

GameObject::GameObject()
	:m_pTransform(new TransformComponent(0.f, 0.f)),
	m_IsActive(true)
{
	AddComponent(m_pTransform);
}

GameObject::~GameObject()
{
	for (auto pChild : m_pChildren)
	{
		delete pChild;
	}
	m_pChildren.clear();
	
	for (auto comp : m_pComponents)
	{
		delete comp;
	}
	m_pComponents.clear();
}

void GameObject::Update(float deltaTime)
{
	for (auto& c : m_pComponents)
	{
		c->Update();
	}

	for (auto pChild : m_pChildren)
	{
		pChild->Update(deltaTime);
	}
}

void GameObject::AddChild(GameObject* pChild)
{
	m_pChildren.push_back(pChild);
}

void GameObject::Render() const
{
	if (HasComponent<RendererComponent>())
	{
		GetComponent<RendererComponent>()->Render(*GetComponent<TransformComponent>());
	}

	if (HasComponent<TextRendererComponent>())
	{
		GetComponent<TextRendererComponent>()->RenderNoScaling(*GetComponent<TransformComponent>());
	}

	for (auto child : m_pChildren)
	{
		child->Render();
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