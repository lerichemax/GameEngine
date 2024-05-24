#include "PCH.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "Subject.h"

#include <algorithm>

#include "Scene.h"

GameObject::GameObject(std::weak_ptr<Coordinator> pRegistry)
	:m_pRegistry(pRegistry), 
	m_Entity(pRegistry.lock()->CreateEntity()),
	m_bIsActive(true),
	m_bIsDestroyed(false),
	m_pTransform(nullptr),
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
	m_pScene(nullptr),
	m_pSubject(new Subject{*other.m_pSubject}),
	m_Tag(other.m_Tag)
{

}

GameObject::~GameObject()
{
	m_pRegistry->DestroyEntity(m_Entity);

	SafeDelete(m_pSubject);
}

void GameObject::Update()
{

}

void GameObject::Refresh()
{
	
	//for (auto& pChild : m_pChildren)
	//{
	//	pChild->Refresh();
	//	if (pChild->m_bIsDestroyed)
	//	{
	//		SafeDelete(pChild);
	//	}
	//}
}

void GameObject::AddChild(std::shared_ptr<GameObject> pChild)
{
	//m_pChildren.push_back(pChild); //remove ?
	//pChild->m_pParent = std::shared_ptr<GameObject>(this); //remove ?

	pChild->m_pTransform->m_pParent = m_pTransform;
	m_pRegistry->AddChild(m_Entity, pChild->m_Entity);

	//pChild->m_pScene = m_pScene; //remove ?
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

	m_pRegistry->SetEntityHierarchyActive(m_Entity, active);
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

	}
}

std::shared_ptr<GameObject> GameObject::FindTagInChildren(std::string const& tag)
{
	printf(tag.c_str());
	return nullptr;
}

void GameObject::Destroy()
{
	m_bIsDestroyed = true;
	m_bIsActive = false;
}