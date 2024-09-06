#pragma once
#include "Component.h"

#define ENEMY_TAG "Enemy"

enum class EnemyType
{
	SlickSam,
	Coily,
	WrongWay
};


class Command;
struct AiControllerComponent : public Component
{
	float const MOVE_INTERVAL{ 0.5f };
	float MoveTimer;

	EnemyType Type;

	Command* pAiCommand;


	void Serialize(StreamWriter& writer) const override;
};