#include "PCH.h"
#include "ColliderComponent.h"

using namespace ecs;

ColliderComponent::ColliderComponent()
	:bIsTrigger{},
	bDraw{},
	m_pShape{nullptr}
{
}

void ColliderComponent::SetShape(std::unique_ptr<geo::Shape> pNewShape)
{
	m_pShape = std::move(pNewShape);
	m_Offset = m_pShape->Pos;
}

geo::Shape* const ColliderComponent::GetShape() const
{
	return m_pShape.get();
}

void ColliderComponent::TriggerEnter(Entity other)
{
	OnTriggerEnter.Notify(other);
	m_OverlappingColliders.insert(other);
}

void ColliderComponent::TriggerExit(Entity other)
{
	OnTriggerExit.Notify(other);
	m_OverlappingColliders.erase(other);
}

void ColliderComponent::Collide(Entity other)
{
	OnCollision.Notify(other);
}

