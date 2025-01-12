#pragma once
#include "Component.h"

struct PlayerComponent final : public ecs::Component
{
	PlayerComponent();
	PROPERTY(float, MoveSpeed);
	PROPERTY(float, ShootDelay);

};

SERIALIZE_CLASS(PlayerComponent, ecs::Component)