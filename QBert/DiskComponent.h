#pragma once
#include "Component.h"

struct DiskComponent : public ecs::Component
{
	float const OFFSET{ -45 };
	float const DROP_MAX_TIME{ 0.75f };
	float const MOVE_SPEED{ 100.f };

	static int const POINTS{ 50 };

	glm::vec2 TargetPosition;
	
	Entity LinkedQube;

	float DropTimer;

	Entity QbertEntity{NULL_ENTITY};
	bool bHasReachedTop;
};

SERIALIZE_CLASS(DiskComponent, ecs::Component)