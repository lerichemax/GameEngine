#include "PCH.h"
#include "CircleCollider.h"

#include "BoxCollider.h"

CircleCollider::CircleCollider(glm::vec2 const& center, unsigned int radius)
	:ColliderComponent(Type::Circle),
	m_pCircle(new Circle(center, radius))
{
	
}

CircleCollider::CircleCollider(CircleCollider const& other)
	:ColliderComponent(other),
	m_pCircle(new Circle(other.m_pCircle->center, other.m_pCircle->radius))
{
}

CircleCollider::~CircleCollider()
{
	SafeDelete(m_pCircle);
}

void CircleCollider::CheckOverlap(ColliderComponent* pOther)
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

bool CircleCollider::IsOverlapping(BoxCollider* pOther) const
{
	return AreOverlapping(pOther->GetRectangle(), *m_pCircle);
}

bool CircleCollider::IsOverlapping(CircleCollider* pOther) const
{
	return AreOverlapping(*m_pCircle, pOther->GetCircle());
}