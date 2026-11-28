#pragma once
#include "System.h"

class EnemySystem final : public ecs::System
{
protected:
	void Start() override;
	void Update() override;
};
