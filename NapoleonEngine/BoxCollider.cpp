#include "PCH.h"
#include "BoxCollider.h"


#include "GameObject.h"
#include "Shapes.h"

empire::BoxCollider::BoxCollider(unsigned int width, unsigned int height)
	:ColliderComponent(Type::Box),
	m_pRectangle(new Rectangle{ 0,0,width, height })
{
}

empire::BoxCollider::BoxCollider(BoxCollider const& other)
	:ColliderComponent(other),
	m_pRectangle(new Rectangle(other.m_pRectangle->pos, other.m_pRectangle->width, other.m_pRectangle->height))
{
	
}

empire::BoxCollider::~BoxCollider()
{
	SafeDelete(m_pRectangle);
}

void empire::BoxCollider::Initialize()
{
	m_pRectangle->pos = m_pGameObject->GetTransform()->GetPosition();
	ColliderComponent::Initialize();
}

void empire::BoxCollider::Update()
{
	m_pRectangle->pos = m_pGameObject->GetTransform()->GetPosition();
}

void empire::BoxCollider::CheckOverlap(ColliderComponent* pOther)
{
	bool isOverlapping = false;
	switch (pOther->GetType())
	{
	case Type::Box:
		isOverlapping = IsOverlapping(static_cast<BoxCollider*>(pOther));
		break;
	case Type::Circle:
		//isOverlapping = IsOverlapping(static_cast<CircleCollider*>(pOther));
		break;
	}

	if (isOverlapping)
	{
		if (std::find(m_pOverlappingColliders.begin(), m_pOverlappingColliders.end(), pOther) 
			!= m_pOverlappingColliders.end() && m_bIsTrigger && m_OnTriggerStay)
		{
			m_OnTriggerStay(m_pGameObject, pOther->GetGameObject());
		}
		else if(m_OnTriggerEnter && m_bIsTrigger)
		{
			m_OnTriggerEnter(m_pGameObject, pOther->GetGameObject());
			m_pOverlappingColliders.emplace_back(pOther);
		}
		else if(m_OnCollision && !m_bIsTrigger)
		{
			m_OnCollision(m_pGameObject, pOther->GetGameObject());
		}
	}
	else if(std::find(m_pOverlappingColliders.begin(), m_pOverlappingColliders.end(), pOther)
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

bool empire::BoxCollider::IsOverlapping(BoxCollider* pOther)
{
	return m_pRectangle->IsOverlapping(pOther->GetRectangle());
}
