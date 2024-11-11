#pragma once
#include "System.h"
#include "Event.h"

class LivesSystem final : public ecs::System {

public:
	LivesSystem() = default;

	EventHandler<LivesSystem, Entity, int> OnDied;
	EventHandler<LivesSystem, Entity> OnPlayerDied;

protected:
	void Update() override;
};