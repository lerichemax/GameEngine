#include "PCH.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "Subject.h"

#include <algorithm>

#include "Scene.h"

GameObject::GameObject() //TO REMOVE
	:m_Entity(0),
	m_bIsActive(true),
	m_bIsDestroyed(false),
	m_pComponents(),
	m_pTransform(nullptr),
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
	m_pComponents(),
	m_pTransform(nullptr),
	m_pChildren(),
	m_pParent(nullptr),
	m_pScene(nullptr),
	m_pSubject(new Subject{}),
	m_Tag()
{
	ECS_TransformComponent transform;
	m_pRegistry->AddComponent<ECS_TransformComponent>(m_Entity, transform);
	m_pTransform = m_pRegistry->GetComponent<ECS_TransformComponent>(m_Entity).lock();
}

GameObject::GameObject(const GameObject& other)
	:m_Entity(m_pRegistry->CreateEntity()),
	m_bIsActive(true),
	m_bIsDestroyed(false),
	m_pComponents(),
	m_pChildren(),
	m_pParent(nullptr),
	m_pScene(nullptr),
	m_pSubject(new Subject{*other.m_pSubject}),
	m_Tag(other.m_Tag)
{

}

GameObject::~GameObject()
{
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
	
	//for (auto& pChild : m_pChildren)
	//{
	//	pChild->Refresh();
	//	if (pChild->m_bIsDestroyed)
	//	{
	//		SafeDelete(pChild);
	//	}
	//}

	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), nullptr), m_pChildren.end());
}

void GameObject::AddChild(std::shared_ptr<GameObject> pChild)
{
	m_pChildren.push_back(pChild); //remove ?
	pChild->m_pParent = std::shared_ptr<GameObject>(this); //remove ?

	pChild->m_pTransform->m_pParent = m_pTransform;

	pChild->m_pScene = m_pScene; //remove ?
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
	for (std::shared_ptr<GameObject> pChild : m_pChildren)
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
		for (auto pChild : m_pChildren)
		{
			pChild->SetTag(tag, applyToChildren);
		}
	}
}

std::shared_ptr<GameObject> GameObject::FindTagInChildren(std::string const& tag)
{
	for (auto pChild : m_pChildren)
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