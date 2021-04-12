#include "PCH.h"
#include "GameObject.h"

#include <algorithm>

#include "RendererComponent.h"
#include "TextRendererComponent.h"
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
	pComp->Init(this);
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

void GameObject::Refresh()
{
	std::for_each(m_pChildren.begin(), m_pChildren.end(), [](GameObject* pChild)
		{
			pChild->Refresh();
		});
	
	m_pChildren.erase(std::remove_if(m_pChildren.begin(), m_pChildren.end(), [](GameObject* pChild)
		{
			return !pChild->IsActive();
		}), m_pChildren.end());
}