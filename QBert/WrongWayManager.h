#pragma once
#include "EnemySpawnerSystem.h"

class WrongWayManager final : public EnemySpawnerSystem
{
public:
	WrongWayManager() = default;
	~WrongWayManager() = default;

protected:
	void Spawn();
};