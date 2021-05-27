#include "PCH.h"
#include "GameObject.h"

#include "RendererComponent.h"
#include "TextRendererComponent.h"

#include <algorithm>
using namespace empire;

GameObject::GameObject()
	:m_pTransform(new TransformComponent(0.f, 0.f)),
	m_bIsActive(true),
	m_bIsInitialized(false),
	m_pScene(nullptr)
{
	AddComponent(m_pTransform);
}

GameObject::GameObject(const GameObject& other)
	:m_pTransform(nullptr),
	m_bIsActive(true),
	m_bIsInitialized(other.m_bIsInitialized),
	m_pScene(nullptr)
{
	for (Component* pComp : other.m_pComponents)
	{
		AddComponent(pComp->Clone());
	}
	m_pTransform = GetComponent<TransformComponent>();
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
	if (m_pChildren.empty())
	{
		return;
	}
	
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
	m_pChildren.push_back(pChild);
	pChild->m_pParent = this;
	
	if (m_bIsInitialized)
	{
		pChild->m_pScene = m_pScene;
		pChild->Initialize();
	}
}

void GameObject::AddComponent(Component* pComp)
{
	if (typeid(*pComp) == typeid(TransformComponent) && HasComponent<TransformComponent>())
	{
		Debugger::GetInstance().Log("Game object already contains a Transform component");
		return;
	}

	m_pComponents.emplace_back(pComp);
	pComp->RootInitialize(this);
}

void GameObject::Initialize()
{
	if (!m_bIsInitialized)
	{
		m_bIsInitialized = true;
		for (auto pComp : m_pComponents)
		{
			pComp->Initialize();
		}

		for (auto pChild : m_pChildren)
		{
			pChild->Initialize();
		}
		
	}
}
