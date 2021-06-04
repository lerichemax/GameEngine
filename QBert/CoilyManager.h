#pragma once
#include "EnemyManager.h"

class CoilyManager final : public EnemyManager
{
public:
	CoilyManager(int maxNbr, float spawnInterval);
	CoilyManager* Clone() const override { return new CoilyManager(*this); }

	CoilyManager(CoilyManager&& other) = delete;
	CoilyManager& operator=(CoilyManager const& rhs) = delete;
	CoilyManager& operator=(CoilyManager&& rhs) = delete;
	~CoilyManager() = default;

	void SetIdle(bool isIdle);

protected:
	void SpawnerTimer() override;

private:
	CoilyManager(CoilyManager const& other);
};