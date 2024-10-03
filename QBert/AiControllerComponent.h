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
struct AiControllerComponent : public Component
{
	float const MOVE_INTERVAL{ 0.5f };
	float MoveTimer;

	int PointsForKill;

	EnemyType Type;

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;
};