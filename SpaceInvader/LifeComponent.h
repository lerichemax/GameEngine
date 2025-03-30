#pragma once
#include "Component.h"
#include "Animation.h"

struct LifeComponent final : ecs::Component
{
	PROPERTY(Animation*, pDeathAnimation);
	bool bIsHit = false;
	bool bIsDead = false;
};

SERIALIZE_CLASS(LifeComponent, ecs::Component)