#pragma once
#include "Component.h"

#include "Event.h"

#include <set>

struct geo::Shape;
class ColliderComponent : public Component
{
	friend class CollisionSystem;

	typedef EventHandler<ColliderComponent, Entity> CollisionEvent;

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
	std::set<Entity> OverlappingColliders;

	void TriggerEnter(Entity other);
	void TriggerExit(Entity other);
	void Collide(Entity other);
};
