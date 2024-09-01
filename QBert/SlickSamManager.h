#pragma once
#include "EnemyManagerSystem.h"

class SlickSamManager final : public EnemySpawnerSystem
{
public:
	explicit SlickSamManager(int maxNbr, float spawnInterval);
	
	SlickSamManager(SlickSamManager&& other) = delete;
	SlickSamManager& operator=(SlickSamManager const& rhs) = delete;
	SlickSamManager& operator=(SlickSamManager&& rhs) = delete;
	~SlickSamManager() = default;

protected:
	void Spawn() override;

private:
	SlickSamManager(SlickSamManager const& other);
};