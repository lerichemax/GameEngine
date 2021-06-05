#pragma once
#include "EnemyManager.h"

class SlickSamManager final : public EnemyManager
{
public:
	explicit SlickSamManager(int maxNbr, float spawnInterval);
	SlickSamManager* Clone() const override { return new SlickSamManager(*this); }
	
	SlickSamManager(SlickSamManager&& other) = delete;
	SlickSamManager& operator=(SlickSamManager const& rhs) = delete;
	SlickSamManager& operator=(SlickSamManager&& rhs) = delete;
	~SlickSamManager() = default;

protected:
	void SpawnerTimer() override;

private:
	SlickSamManager(SlickSamManager const& other);
};