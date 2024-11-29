#include "PCH.h"
#include "ColliderComponent.h"

using namespace ecs;

ColliderComponent::ColliderComponent()
	:bIsTrigger{},
	bDraw{},
	pShape{nullptr}
{
}

void ColliderComponent::SetShape(geo::Shape* pNewShape)
{
	assert(pShape == nullptr && "Can't reassign shape (yet)");

	pShape = std::unique_ptr<geo::Shape>(std::forward<geo::Shape*>(pNewShape));
}

geo::Shape* const ColliderComponent::GetShape() const
{
	return pShape.get();
}

void ColliderComponent::TriggerEnter(Entity other)
{
	OnTriggerEnter.Notify(other);
	OverlappingColliders.insert(other);
}

void ColliderComponent::TriggerExit(Entity other)
{
	OnTriggerExit.Notify(other);
	OverlappingColliders.erase(other);
}

void ColliderComponent::Collide(Entity other)
{
	OnCollision.Notify(other);
}

