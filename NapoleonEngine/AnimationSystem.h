#pragma once
#include "System.h"

class AnimationSystem final : public ecs::System
{
protected:
	void Update() override;
};