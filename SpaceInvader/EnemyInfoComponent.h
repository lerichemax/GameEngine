#pragma once
#include "Component.h"

struct EnemyInfoComponent final : public ecs::Component
{
	std::string EnemyPrefab;
	int AmountPerRows = 11;
	int Rows = 1;
};
