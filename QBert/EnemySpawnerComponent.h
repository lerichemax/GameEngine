#pragma once
#include "Component.h"

struct EnemySpawnerComponent : public Component
{
	unsigned int MaxEnemies;
	float SpawnInterval;

	std::string EnemyToSpawn;

	unsigned int NbrEnemies;

	float EnemySpawnTimer;

	void Serialize(StreamWriter& writer) const override;
};