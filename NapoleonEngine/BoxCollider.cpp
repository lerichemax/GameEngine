#include "PCH.h"
#include "BoxCollider.h"


#include "GameObject.h"
#include "Shapes.h"

empire::BoxCollider::BoxCollider(float width, float height)
	:ColliderComponent(Type::Box),
	m_pRectangle(new Rectangle{ 0,0,width, height })
{
	
}

void empire::BoxCollider::Initialize()
{
	m_pRectangle->pos = m_pGameObject->GetTransform()->GetPosition();
	ColliderComponent::Initialize();
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
		
	}
	
}

bool empire::BoxCollider::IsOverlapping(BoxCollider* pOther)
{
	return m_pRectangle->IsOverlapping(*pOther->m_pRectangle);
}
