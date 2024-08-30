#include "PCH.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Shapes.h"

BoxCollider::BoxCollider(unsigned int width, unsigned int height)
	:ColliderComponent(Type::Box),
	m_pRectangle(new geo::Rectangle{ 0,0,width, height })
{
}

BoxCollider::BoxCollider(BoxCollider const& other)
	:ColliderComponent(other),
	m_pRectangle(new geo::Rectangle(other.m_pRectangle->pos, other.m_pRectangle->width, other.m_pRectangle->height))
{
	
}

BoxCollider::~BoxCollider()
{
	SAFE_DELETE(m_pRectangle);
}

void BoxCollider::Initialize()
{
	m_pRectangle->pos = GetGameObject()->GetTransform()->GetLocation();
	ColliderComponent::Initialize();
}

void BoxCollider::Update()
{
	m_pRectangle->pos = GetGameObject()->GetTransform()->GetLocation();
}

void BoxCollider::CheckOverlap(ColliderComponent* pOther)
{
	bool isOverlapping = false;
	switch (pOther->GetType())
	{
	case Type::Box:
		isOverlapping = IsOverlapping(static_cast<BoxCollider*>(pOther));
		break;
	case Type::Circle:
		isOverlapping = IsOverlapping(static_cast<CircleCollider*>(pOther));
		break;
	}

	CallOverlapFunctions(isOverlapping, pOther);
}

bool BoxCollider::IsOverlapping(BoxCollider* pOther) const
{
	return geo::AreOverlapping(*m_pRectangle, pOther->GetRectangle());
}

bool BoxCollider::IsOverlapping(CircleCollider* pOther) const
{
	return geo::AreOverlapping(*m_pRectangle, pOther->GetCircle());
}