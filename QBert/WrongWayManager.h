#pragma once
#include "EnemyManager.h"

class WrongWayManager final : public EnemyManager
{
public:
	WrongWayManager(int maxNbr, float spawnInterval);
	WrongWayManager* Clone() override { return new WrongWayManager(*this); }

	WrongWayManager(WrongWayManager&& other) = delete;
	WrongWayManager& operator=(WrongWayManager const& rhs) = delete;
	WrongWayManager& operator=(WrongWayManager&& rhs) = delete;
	~WrongWayManager() = default;

protected:
	void SpawnerTimer() override;

private:
	WrongWayManager(WrongWayManager const& other);
};