#pragma once
#include "Component.h"

enum class ConnectionDirection;
struct CoilyComponent : public Component
{
	CoilyComponent();

	unsigned int static const MOVEMENT_QUEUE_SIZE{ 2 };

	ConnectionDirection MovementQueue[MOVEMENT_QUEUE_SIZE]{};

	int CurrentlyInQueue{};

	void Serialize(StreamWriter& writer) const override;
};