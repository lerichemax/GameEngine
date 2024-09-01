#pragma once
#include "EnemyManager.h"

class Coily;
class CoilyManager final : public EnemySpawnerSystem
{
public:
	explicit CoilyManager(int maxNbr, float spawnInterval);

	CoilyManager(CoilyManager&& other) = delete;
	CoilyManager& operator=(CoilyManager const& rhs) = delete;
	CoilyManager& operator=(CoilyManager&& rhs) = delete;
	~CoilyManager() = default;

	void SetIdle(bool isIdle);

protected:
	void Spawn() override;

private:
	CoilyManager(CoilyManager const& other);

};