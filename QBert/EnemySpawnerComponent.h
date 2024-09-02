#pragma once
#include "Component.h"

struct EnemySpawnerComponent : public Component
{
	unsigned int MaxEnemies;
	float SpawnInterval;

	unsigned int NbrEnemies;

	float EnemySpawnTimer;

	std::vector<Entity> SpawnedEnemies;

	void Serialize(StreamWriter& writer) const override;
};