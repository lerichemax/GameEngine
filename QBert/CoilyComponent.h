#pragma once
#include "Component.h"

enum class ConnectionDirection;
struct CoilyComponent : public ecs::Component
{
	CoilyComponent();

	unsigned int static const MOVEMENT_QUEUE_SIZE{ 3 };

	ConnectionDirection MovementQueue[MOVEMENT_QUEUE_SIZE]{};

	int CurrentlyInQueue{};
};

SERIALIZE_CLASS(CoilyComponent, ecs::Component)