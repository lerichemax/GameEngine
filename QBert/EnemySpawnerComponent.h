#pragma once
#include "Component.h"

enum class EnemyType;
struct EnemySpawnerComponent : public ecs::Component
{
	unsigned int MaxEnemies;
	float SpawnInterval;

	unsigned int NbrEnemies;

	float EnemySpawnTimer;

	EnemyType Type;

	std::vector<Entity> SpawnedEnemies;
};