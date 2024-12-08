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

	int PointsForKill;

	EnemyType Type;

	void Serialize(StreamWriter& writer) const;
	void Deserialize(JsonReader* const reader, SerializationMap& context);
};