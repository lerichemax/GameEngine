#include "PCH.h"
#include "EnemyManager.h"

#include "Enemy.h"
#include "PrefabsManager.h"
#include "Timer.h"

EnemyManager::EnemyManager(unsigned int maxNbr, float spawnInterval)
	:MAX_ENEMY_OF_TYPE{ maxNbr },
	SPAWN_INTERVAL(spawnInterval),
	m_NbrEnemies(),
	m_EnemySpawnTimer(),
	m_pPyramid(nullptr),
	m_pObserver(nullptr),
	m_pEnemies()
{
}

EnemyManager::EnemyManager(EnemyManager const& other)
	:MAX_ENEMY_OF_TYPE(other.MAX_ENEMY_OF_TYPE),
	SPAWN_INTERVAL(other.SPAWN_INTERVAL),
	m_NbrEnemies(other.m_NbrEnemies),
	m_EnemySpawnTimer(other.m_EnemySpawnTimer),
	m_pPyramid(other.m_pPyramid),
	m_pObserver(other.m_pObserver),
	m_pEnemies()
{
	for (size_t i = 0; i < MAX_ENEMY_OF_TYPE; i++)
	{
		m_pEnemies[i] = other.m_pEnemies[i]->Clone();
	}
}

void EnemyManager::Initialize()
{
	m_pEnemies.reserve(MAX_ENEMY_OF_TYPE);
	m_pEnemies = std::vector<Enemy*>(MAX_ENEMY_OF_TYPE, nullptr);
}


void EnemyManager::Update()
{
	SpawnerTimer();
}

void EnemyManager::SpawnerTimer()
{
	if (m_NbrEnemies < MAX_ENEMY_OF_TYPE)
	{
		if (m_EnemySpawnTimer < SPAWN_INTERVAL)
		{
			m_EnemySpawnTimer += Timer::GetInstance().GetDeltaTime();
		}
		else
		{
			Spawn();
			m_EnemySpawnTimer = 0;
			m_NbrEnemies++;
		}
	}
}

void EnemyManager::EnemyDied(Enemy* pEnemy)
{
	if (!m_pEnemies.empty())
	{
		for (size_t i{}; i < MAX_ENEMY_OF_TYPE; i++)
		{
			if (m_pEnemies[i] == pEnemy)
			{
				m_pEnemies[i] = nullptr;
				break;
			}
		}
	}
	m_NbrEnemies--;
}

void EnemyManager::Reset()
{
	if (!m_pEnemies.empty())
	{
		for (size_t i{}; i < MAX_ENEMY_OF_TYPE; i++)
		{
			if (m_pEnemies[i] != nullptr)
			{
				m_pEnemies[i]->Die();
			}
		}
	}
	m_NbrEnemies = 0;
}

void EnemyManager::ResetTimer()
{
	m_EnemySpawnTimer = 0;
}

void EnemyManager::AddToArray(Enemy* pCoily)
{
	if (!m_pEnemies.empty())
	{
		for (size_t i = 0; i < MAX_ENEMY_OF_TYPE; i++)
		{
			if (m_pEnemies[i] == nullptr)
			{
				m_pEnemies[i] = pCoily;
				return;
			}
		}
	}
}