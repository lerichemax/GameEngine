#pragma once
#include "Component.h"

enum class EnemyType;
struct EnemySpawnerComponent : public Component
{
	unsigned int MaxEnemies;
	float SpawnInterval;

	unsigned int NbrEnemies;

	float EnemySpawnTimer;

	EnemyType Type;

	std::vector<Entity> SpawnedEnemies;

	void Serialize(StreamWriter& writer) const override;
};