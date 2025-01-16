#pragma once
#include "System.h"
#include "Event.h"

class ColliderComponent;
class CollisionSystem final : public System {

public:
	CollisionSystem() = default;
	~CollisionSystem();

	void Update() override;

	static EventHandler<CollisionSystem, Entity, Entity> TriggerEnter;
	static EventHandler<CollisionSystem, Entity, Entity> TriggerExit;
	static EventHandler<CollisionSystem, Entity, Entity> Collision;

private:
	void HandleOverlapping(ColliderComponent* const pCollider, ColliderComponent* const pOtherCollider);
	void HandleCollision(ColliderComponent* const pCollider, ColliderComponent* const pOtherCollider);
};