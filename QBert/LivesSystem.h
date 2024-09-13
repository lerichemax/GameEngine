#pragma once
#include "System.h"
#include "Event.h"

class LivesSystem final : public System {

public:
	LivesSystem() = default;

	EventHandler<LivesSystem, Entity, int> OnDied;
	EventHandler<LivesSystem> OnGameOver;

	void SetSignature() override;
	void Serialize(StreamWriter& writer) const override;

protected:
	void Update() override;
};