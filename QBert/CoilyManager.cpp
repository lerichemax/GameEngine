#include "PCH.h"
#include "CoilyManager.h"

#include "GameManager.h"
#include "Coily.h"
#include "Pyramid.h"
#include "CoilyCharacterController.h"

#include "ObserverManager.h"
#include "PrefabsManager.h"
#include "Timer.h"


CoilyManager::CoilyManager(int maxNbr, float spawnInterval)
	:EnemyManager(maxNbr, spawnInterval)
{

}

CoilyManager::CoilyManager(CoilyManager const& other)
	: EnemyManager(other)
{

}
void CoilyManager::SpawnerTimer()
{
	//Spawn Coilies
	if (m_NbrEnemies < MAX_ENEMY_OF_TYPE)
	{
		if (m_EnemySpawnTimer < SPAWN_INTERVAL)
		{
			m_EnemySpawnTimer += empire::Timer::GetInstance().GetDeltaTime();
			return;
		}

		int random{ rand() % 2 + 1 };
		GameObject* pCoily = PrefabsManager::GetInstance().Instantiate("Coily");
		m_pPyramid->GetGameObject()->AddChild(pCoily);
		pCoily->GetComponent<CoilyCharacterController>()->SetPyramid(m_pPyramid);
		pCoily->GetComponent<Coily>()->SetCurrentQube(m_pPyramid->GetQubes()[random]);

		AddToArray(pCoily->GetComponent<Coily>());
		if (m_pObserver != nullptr)
		{
			pCoily->AddObserver(m_pObserver);
		}
		m_EnemySpawnTimer = 0;
		m_NbrEnemies++;
		
	}
}

void CoilyManager::SetIdle(bool isIdle)
{
	for (size_t i{}; i < MAX_ENEMY_OF_TYPE;++i)
	{
		if (m_pEnemies[i] != nullptr)
		{
			m_pEnemies[i]->GetGameObject()->GetComponent<CoilyCharacterController>()->SetIdle(isIdle);
		}
	}
}