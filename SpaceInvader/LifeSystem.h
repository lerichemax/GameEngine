#pragma once
#include "System.h"

class LifeSystem final : public ecs::System
{
protected:
	void Update() override;
};