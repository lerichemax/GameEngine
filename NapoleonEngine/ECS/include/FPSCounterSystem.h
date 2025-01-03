#pragma once
#include "System.h"

class FPSCounterSystem : public ecs::System
{
protected:
	void Update() override;
};