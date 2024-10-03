#include "PCH.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Shapes.h"

BoxCollider::BoxCollider(unsigned int Width, unsigned int Height)
{
}

BoxCollider::BoxCollider(BoxCollider const& other)
{
	
}

BoxCollider::~BoxCollider()
{
	SAFE_DELETE(m_pRectangle);
}

void BoxCollider::Initialize()
{
	//m_pRectangle->Pos = GetGameObject()->GetTransform()->GetLocation();
}

void BoxCollider::Update()
{
	//m_pRectangle->Pos = GetGameObject()->GetTransform()->GetLocation();
}

void BoxCollider::CheckOverlap(ColliderComponent* pOther)
{
	//bool isOverlapping = false;
	//switch (pOther->GetType())
	//{
	//case Type::Box:
	//	isOverlapping = IsOverlapping(static_cast<BoxCollider*>(pOther));
	//	break;
	//case Type::Circle:
	//	isOverlapping = IsOverlapping(static_cast<CircleCollider*>(pOther));
	//	break;
	//}

	//CallOverlapFunctions(isOverlapping, pOther);
}

bool BoxCollider::IsOverlapping(BoxCollider* pOther) const
{
	return geo::AreOverlapping(*m_pRectangle, pOther->GetRectangle());
}

bool BoxCollider::IsOverlapping(CircleCollider* pOther) const
{
	return geo::AreOverlapping(*m_pRectangle, pOther->GetCircle());
}