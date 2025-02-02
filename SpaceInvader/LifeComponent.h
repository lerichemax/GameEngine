#pragma once
#include "Component.h"

struct LifeComponent final : ecs::Component
{
	bool bIsHit = false;
	bool bIsDead = false;
};