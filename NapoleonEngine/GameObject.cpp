#include "PCH.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "Subject.h"

#include <algorithm>

#include "Scene.h"

using namespace empire;

GameObject::GameObject()
	:m_bIsActive(true),
	m_bIsDestroyed(false),
	m_bIsInitialized(false),
	m_pComponents(),
	m_pTransform(new TransformComponent(0.f, 0.f)),
	m_pChildren(),
	m_pParent(nullptr),
	m_pScene(nullptr),
	m_pSubject(new Subject{}),
	m_Tag()
{
	AddComponent(m_pTransform);
}

GameObject::GameObject(const GameObject& other)
	:m_bIsActive(true),
	m_bIsDestroyed(false),
	m_bIsInitialized(other.m_bIsInitialized),
	m_pComponents(),
	m_pTransform(nullptr),
	m_pChildren(),
	m_pParent(nullptr),
	m_pScene(nullptr),
	m_pSubject(new Subject{*other.m_pSubject}),
	m_Tag(other.m_Tag)
{
	for (Component* pComp : other.m_pComponents)
	{
		AddComponent(pComp->Clone());
	}
	for (GameObject* pChild : other.m_pChildren)
	{
		auto newChild = new GameObject(*pChild);
		m_pChildren.push_back(newChild);
		newChild->m_pParent = this;
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

	SafeDelete(m_pSubject);
}

void GameObject::Update()
{
	for (auto& c : m_pComponents)
	{
		if (c->IsEnable())
		{
			c->Update();
		}
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
		if (pChild->m_bIsDestroyed)
		{
			SafeDelete(pChild);
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

void GameObject::AddObserver(Observer* pObserver)
{
	m_pSubject->AddObserver(pObserver);
}

void GameObject::RemoveObserver(Observer* pObserver)
{
	m_pSubject->RemoveObserver(pObserver);
}

void GameObject::Notify(int event)
{
	m_pSubject->Notify(this, event);
}
void GameObject::SetActive(bool active)
{
	m_bIsActive = active;
	for (GameObject* pChild : m_pChildren)
	{
		pChild->SetActive(active);
	}
}

bool GameObject::IsActive() const
{
	return m_bIsActive && m_pScene->IsActive();
}

void GameObject::SetTag(std::string const& tag, bool applyToChildren)
{
	m_Tag = tag;
	if (applyToChildren)
	{
		for (GameObject* pChild : m_pChildren)
		{
			pChild->SetTag(tag, applyToChildren);
		}
	}
}

GameObject* GameObject::FindTagInChildren(std::string const& tag)
{
	for (GameObject* pChild : m_pChildren)
	{
		if (pChild->GetTag() == tag)
		{
			return pChild;
		}
	}
	return nullptr;
}

void GameObject::Destroy()
{
	m_bIsDestroyed = true;
	m_bIsActive = false;
}