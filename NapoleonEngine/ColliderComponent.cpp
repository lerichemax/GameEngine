#include "PCH.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "Scene.h"

empire::ColliderComponent::ColliderComponent(Type type)
	:m_Type(type),
	m_pOverlappingColliders(),
	m_OnTriggerEnter(nullptr),
	m_OnTriggerStay(nullptr),
	m_OnTriggerExit(nullptr),
	m_OnCollision(nullptr),
	m_bIsTrigger(false)
{
}

empire::ColliderComponent::~ColliderComponent()
{
	if (m_pGameObject->IsInitialized())
	{
		m_pGameObject->GetParentScene()->RemoveCollider(this);
	}	
}

void empire::ColliderComponent::Initialize()
{
	m_pGameObject->GetParentScene()->m_pColliders.emplace_back(this);
}

void empire::ColliderComponent::CallOverlapFunctions(bool isOverlapping, ColliderComponent* pOther)
{
	if (isOverlapping)
	{
		if (std::find(m_pOverlappingColliders.begin(), m_pOverlappingColliders.end(), pOther)
			!= m_pOverlappingColliders.end() && m_bIsTrigger && m_OnTriggerStay)
		{
			m_OnTriggerStay(m_pGameObject, pOther->GetGameObject());
		}
		else if (m_OnTriggerEnter && m_bIsTrigger)
		{
			m_OnTriggerEnter(m_pGameObject, pOther->GetGameObject());
			m_pOverlappingColliders.emplace_back(pOther);
		}
		else if (m_OnCollision && !m_bIsTrigger)
		{
			m_OnCollision(m_pGameObject, pOther->GetGameObject());
		}
	}
	else if (std::find(m_pOverlappingColliders.begin(), m_pOverlappingColliders.end(), pOther)
		!= m_pOverlappingColliders.end() && !m_bIsTrigger)
	{
		if (m_OnTriggerExit)
		{
			m_OnTriggerExit(m_pGameObject, pOther->GetGameObject());
		}
		m_pOverlappingColliders.erase(std::remove(m_pOverlappingColliders.begin(),
			m_pOverlappingColliders.end(), pOther));
	}
}