#include "PCH.h"
#include "GameObject.h"

#include "RendererComponent.h"
#include "TextRendererComponent.h"

#include <algorithm>
using namespace empire;

GameObject::GameObject()
	:m_pTransform(new TransformComponent(0.f, 0.f)),
	m_IsActive(true),
	m_pScene(nullptr)
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

void GameObject::Update()
{
	for (auto& c : m_pComponents)
	{
		c->Update();
	}

	for (auto pChild : m_pChildren)
	{
		pChild->Update();
	}
}

void GameObject::Refresh()
{
	for (auto& pChild : m_pChildren)
	{
		pChild->Refresh();
		if (!pChild->IsActive())
		{
			delete pChild;
			pChild = nullptr;
		}
	}

	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), nullptr), m_pChildren.end());
}

void GameObject::AddChild(GameObject* pChild)
{
	//pChild->m_pParent = this;
	m_pChildren.push_back(pChild);
	pChild->m_pScene = m_pScene;
}

void GameObject::AddComponent(Component* pComp)
{
	if (typeid(*pComp) == typeid(TransformComponent) && HasComponent<TransformComponent>())
	{
		std::cout << "Game object already contains a Transform component\n";
		return;
	}

	m_pComponents.emplace_back(pComp);
	pComp->RootInitialize(this);
}

void GameObject::Initialize()
{
	for (auto pComp : m_pComponents)
	{
		pComp->Initialize();
	}

	for (auto pChild : m_pChildren)
	{
		pChild->Initialize();
	}
}

void GameObject::Render() const
{
	//if (HasComponent<RendererComponent>())
	//{
	//	GetComponent<RendererComponent>()->Render(*GetComponent<TransformComponent>());
	//}

	//if (HasComponent<TextRendererComponent>())
	//{
	//	GetComponent<TextRendererComponent>()->RenderNoScaling(*GetComponent<TransformComponent>());
	//}

	//for (auto child : m_pChildren)
	//{
	//	child->Render();
	//}
}