#include "PCH.h"
#include "BoxCollider.h"

#include "TransformComponent.h"
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

	CallOverlapFunctions(isOverlapping, pOther);
}

bool empire::BoxCollider::IsOverlapping(BoxCollider* pOther)
{
	return m_pRectangle->IsOverlapping(pOther->GetRectangle());
}
