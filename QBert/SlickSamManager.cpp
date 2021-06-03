#include "PCH.h"
#include "SlickSamManager.h"

#include "GameManager.h"
#include "SlickSam.h"
#include "Pyramid.h"

#include "ObserverManager.h"
#include "PrefabsManager.h"
#include "Timer.h"


SlickSamManager::SlickSamManager(int maxNbr, float spawnInterval)
	:EnemyManager(maxNbr, spawnInterval)
{
	
}

SlickSamManager::SlickSamManager(SlickSamManager const& other)
	:EnemyManager(other)
{
	
}
void SlickSamManager::SpawnerTimer()
{
	//Spawn Slick and Sam
	if (m_NbrEnemies < MAX_ENEMY_OF_TYPE)
	{
		if (m_EnemySpawnTimer < SPAWN_INTERVAL)
		{
			m_EnemySpawnTimer += empire::Timer::GetInstance().GetDeltaTime();
		}
		else
		{
			int random{ rand() % 2 + 1 };
			GameObject* pSlickSam = PrefabsManager::GetInstance().Instantiate("SlickSam");
			pSlickSam->GetComponent<SlickSam>()->SetCurrentQube(m_pPyramid->GetQubes()[random]);

			AddToArray(pSlickSam->GetComponent<SlickSam>());
			m_pPyramid->GetGameObject()->AddChild(pSlickSam);
			if (m_pObserver != nullptr)
			{
				pSlickSam->AddObserver(m_pObserver);
			}
			m_EnemySpawnTimer = 0;
			m_NbrEnemies++;
		}
	}
}