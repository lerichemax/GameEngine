#pragma once
#include "System.h"
#include "Event.h"

class LivesSystem final : public System {

public:
	LivesSystem() = default;

	EventHandler<LivesSystem, Entity, int> OnDied;
	EventHandler<LivesSystem, Entity> OnPlayerDied;

	void SetSignature() const override;
	void Serialize(StreamWriter& writer) const override;

protected:
	void Update() override;
};