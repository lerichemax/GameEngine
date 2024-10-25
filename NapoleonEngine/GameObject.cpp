#include "PCH.h"
#include "GameObject.h"

#include <algorithm>


GameObject::GameObject(Entity entity, Registry* const pRegistry)
	:m_pRegistry(pRegistry),
	m_Entity{entity}
{
}

GameObject::GameObject(GameObject&& other)
	:m_Entity{other.m_Entity},
	m_pRegistry{other.m_pRegistry}
{
}

GameObject& GameObject::operator=(GameObject&& other)
{
	m_Entity = other.m_Entity;
	m_pRegistry = other.m_pRegistry;

	return *this;
}

TransformComponent* const GameObject::GetTransform() const
{ 
	return GetComponent<TransformComponent>(); 
}

void GameObject::AddChild(GameObject* const pChild)
{
	pChild->GetTransform()->SetParent(GetTransform());
	m_pRegistry->AddChild(m_Entity, pChild->m_Entity);
}

void GameObject::AddChild(std::shared_ptr<GameObject> pChild)
{
	AddChild(pChild.get());
}

void GameObject::SetTag(std::string const& tag, bool applyToChildren)
{
	m_pRegistry->SetTag(m_Entity, tag);

	if (!applyToChildren)
	{
		return;
	}

	std::unordered_set<Entity> children = m_pRegistry->GetChildren(GetEntity());

	for (Entity entity : children)
	{
		m_pRegistry->SetTag(entity, tag);
	}
}

void GameObject::SetActive(bool active, bool includeChildren)
{
	if (includeChildren)
	{
		m_pRegistry->SetEntityHierarchyActive(m_Entity, active);
	}
	else
	{
		m_pRegistry->SetEntityActive(m_Entity, active);
	}
}

bool GameObject::IsActive() const
{
	return true;// m_pRegistry->;
}

std::string GameObject::GetTag() const
{ 
	return m_pRegistry->GetTag(m_Entity); 
}

std::shared_ptr<GameObject> GameObject::FindChildrenWithTag(std::string const& tag) const
{
	std::unordered_set<Entity> children = m_pRegistry->GetChildren(m_Entity);

	for (Entity child : children)
	{
		if (m_pRegistry->HasTag(child, tag))
		{
			return std::shared_ptr<GameObject>{new GameObject{ child, m_pRegistry }};
		}
	}

	LOG_WARNING("No GameObject found with tag %s", tag.c_str());
	return nullptr;
}

void GameObject::Destroy()
{
	//m_bIsDestroyed = true;
	//m_bIsActive = false;
}