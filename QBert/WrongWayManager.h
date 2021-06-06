#pragma once
#include "EnemyManager.h"

class WrongWayManager final : public EnemyManager
{
public:
	explicit WrongWayManager(int maxNbr, float spawnInterval);
	WrongWayManager* Clone() const override { return new WrongWayManager(*this); }

	WrongWayManager(WrongWayManager&& other) = delete;
	WrongWayManager& operator=(WrongWayManager const& rhs) = delete;
	WrongWayManager& operator=(WrongWayManager&& rhs) = delete;
	~WrongWayManager() = default;

protected:
	void Spawn() override;

private:
	WrongWayManager(WrongWayManager const& other);
};