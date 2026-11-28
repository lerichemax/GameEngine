#pragma once
#include "Component.h"

struct EnemyGroupComponent final : public ecs::Component
{
	friend class EnemySystem;
public:
	int MoveDirection = 1;
	int MoveDistance = 5;
	float MoveInterval = 0.75f;
	float MinimumMoveInterval = 0.25f;
	float SpeedIncrease = 0.05f;
	int MaxDistance = 50;

private:
	float MoveTimer = 0.f;
	int CurrentDistance = 0;
};
