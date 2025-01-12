#pragma once
#include "System.h"

class PlayerControllerSystem final : public ecs::System
{
protected:
	void Update() override;
};