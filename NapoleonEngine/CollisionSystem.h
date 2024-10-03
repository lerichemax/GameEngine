#pragma once
#include "System.h"

class ColliderComponent;
class CollisionSystem final : public System {

public:
	CollisionSystem() = default;

	void Update() override;

	void SetSignature() const override;
	void Serialize(StreamWriter& writer) const override;

private:
	void HandleOverlapping(ColliderComponent* const pCollider, ColliderComponent* const pOtherCollider);
	void HandleCollision(ColliderComponent* const pCollider, ColliderComponent* const pOtherCollider);
};