#pragma once
#include "System.h"

class ProjectileSystem final : public ecs::System
{
protected:
	void Initialize() override;
	void Update() override;
};