#pragma once
#include "Component.h"

#define ENEMY_TAG "Enemy"

enum class EnemyType
{
	SlickSam,
	Coily,
	WrongWay, 
	MAX
};


class Command;
struct AiControllerComponent : public ecs::Component
{
	float const MOVE_INTERVAL{ 0.5f };
	float MoveTimer;

	PROPERTY(int, PointsForKill);

	PROPERTY(EnemyType, Type);
};

SERIALIZE_CLASS(AiControllerComponent, ecs::Component)