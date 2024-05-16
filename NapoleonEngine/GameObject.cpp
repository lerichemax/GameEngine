#include "PCH.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "Subject.h"

#include <algorithm>

#include "Scene.h"

GameObject::GameObject() //TEMP
	:m_Entity(0),
	m_bIsActive(true),
	m_bIsDestroyed(false),
	m_bIsInitialized(false),
	m_pComponents(),
	m_pTransform(nullptr),
	m_pEcsTransform(new ECS_TransformComponent{}),
	m_pChildren(),
	m_pParent(nullptr),
	m_pScene(nullptr),
	m_pSubject(new Subject{}),
	m_Tag()
{

	//AddComponent(m_pTransform);
}

GameObject::GameObject(std::weak_ptr<Coordinator> pRegistry)
	:m_pRegistry(pRegistry), 
	m_Entity(pRegistry.lock()->CreateEntity()),
	m_bIsActive(true),
	m_bIsDestroyed(false),
	m_bIsInitialized(false),
	m_pComponents(),
	m_pTransform(nullptr),
	m_pEcsTransform(nullptr),
	m_pChildren(),
	m_pParent(nullptr),
	m_pScene(nullptr),
	m_pSubject(new Subject{}),
	m_Tag()
{
	ECS_TransformComponent transform;
	m_pRegistry->AddComponent<ECS_TransformComponent>(m_Entity, transform);
	m_pEcsTransform = m_pRegistry->GetComponent<ECS_TransformComponent>(m_Entity).lock();
}

GameObject::GameObject(const GameObject& other)
	:m_Entity(m_pRegistry->CreateEntity()),
	m_bIsActive(true),
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
	m_pRegistry->DestroyEntity(m_Entity);

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

void GameObject::AddObserver(Observer* pObserver) const
{
	m_pSubject->AddObserver(pObserver);
}

void GameObject::RemoveObserver(Observer* pObserver) const
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
	return m_bIsActive;//&& m_pScene->IsActive();
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