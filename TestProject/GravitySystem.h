#pragma once
#include "System.h"

class GravitySystem final : public ecs::System
{
protected:
	void Update() override;
};
