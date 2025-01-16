#pragma once
#include "Component.h"

#include "Event.h"

#include <set>

struct geo::Shape;
class ColliderComponent : public ecs::Component
{
	friend class CollisionSystem;

	typedef EventHandler<ColliderComponent, ecs::Entity> CollisionEvent;

public:
	ColliderComponent();

	CollisionEvent OnTriggerEnter;
	CollisionEvent OnTriggerExit;
	CollisionEvent OnCollision;

	PROPERTY(bool, bIsTrigger);
	PROPERTY(bool, bDraw);

	void SetShape(std::unique_ptr<geo::Shape> pNewShape);
	geo::Shape* const GetShape() const;

private:
	PROPERTY(std::unique_ptr<geo::Shape>, m_pShape);
	std::set<ecs::Entity> m_OverlappingColliders;
	PROPERTY(glm::vec2, m_Offset);
	glm::vec2 m_PreviousLocation;

	void TriggerEnter(ecs::Entity other);
	void TriggerExit(ecs::Entity other);
	void Collide(ecs::Entity other);
};

SERIALIZE_CLASS(ColliderComponent, ecs::Component)
