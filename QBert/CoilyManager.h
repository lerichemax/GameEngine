#pragma once
#include "EnemySpawnerSystem.h"

class Coily;
class CoilyManager final : public EnemySpawnerSystem
{
public:
	CoilyManager() = default;
	~CoilyManager() = default;

	void SetIdle(bool isIdle);

protected:
	void Spawn();

};