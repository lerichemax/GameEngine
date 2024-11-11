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
	CollisionEvent OnTriggerEnter;
	CollisionEvent OnTriggerExit;
	CollisionEvent OnCollision;

	bool bIsTrigger;
	bool bDraw;

	void SetShape(geo::Shape* pNewShape);
	geo::Shape* const GetShape() const;

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

private:
	std::unique_ptr<geo::Shape> pShape{ nullptr };
	std::set<ecs::Entity> OverlappingColliders;

	void TriggerEnter(ecs::Entity other);
	void TriggerExit(ecs::Entity other);
	void Collide(ecs::Entity other);
};
