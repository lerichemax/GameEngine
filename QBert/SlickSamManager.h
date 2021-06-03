#pragma once
#include "EnemyManager.h"

class SlickSamManager final : public EnemyManager
{
public:
	SlickSamManager(int maxNbr, float spawnInterval);
	SlickSamManager* Clone() override { return new SlickSamManager(*this); }
	
	SlickSamManager(SlickSamManager&& other) = delete;
	SlickSamManager& operator=(SlickSamManager const& rhs) = delete;
	SlickSamManager& operator=(SlickSamManager&& rhs) = delete;
	~SlickSamManager() = default;

protected:
	void SpawnerTimer() override;

private:
	SlickSamManager(SlickSamManager const& other);
};