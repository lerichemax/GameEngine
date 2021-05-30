#include "PCH.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "Scene.h"

empire::ColliderComponent::ColliderComponent(Type type)
	:m_bIsTrigger(false),
	m_Type(type),
	m_OnTriggerEnter(nullptr),
	m_OnTriggerStay(nullptr),
	m_OnTriggerExit(nullptr),
	m_OnCollision(nullptr)
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