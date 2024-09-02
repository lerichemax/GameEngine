#pragma once
#include "EnemySpawnerSystem.h"

class SlickSamManager final : public EnemySpawnerSystem
{
public:
	SlickSamManager() = default;
	~SlickSamManager() = default;

protected:
	void Spawn();

};