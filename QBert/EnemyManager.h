#pragma once
#include "Component.h"


class Pyramid;
class Qube;
class Enemy;
class GameManager;
class EnemyManager : public Component
{
public:
	explicit EnemyManager(unsigned int maxNbr, float spawnInterval);
	virtual EnemyManager* Clone() const override = 0;

	EnemyManager(EnemyManager&& other) = delete;
	EnemyManager& operator=(EnemyManager const& rhs) = delete;
	EnemyManager& operator=(EnemyManager&& rhs) = delete;
	virtual ~EnemyManager() = default;

	void Initialize() override;
	void Update() override;

	void EnemyDied(Enemy* pEnemy);
	void Reset();
	void ResetTimer();

	void SetPyramid(Pyramid* pPyramid) { m_pPyramid = pPyramid; }
	void SetGameManager(GameManager* pManager) { m_pObserver = pManager; }

protected:
	unsigned int const MAX_ENEMY_OF_TYPE;
	float const SPAWN_INTERVAL;

	unsigned int m_NbrEnemies;

	float m_EnemySpawnTimer;

	Pyramid* m_pPyramid;
	GameManager* m_pObserver;

	std::vector<Enemy*> m_pEnemies;
	
	virtual void Spawn() = 0;

	void AddToArray(Enemy* pEnemy);
	
	EnemyManager(EnemyManager const& other);

private:
	void SpawnerTimer();
};