#include "PCH.h"
#include "WrongWayManager.h"

#include "GameManager.h"
#include "WrongWay.h"
#include "Pyramid.h"

#include "ObserverManager.h"
#include "PrefabsManager.h"
#include "Qube.h"
#include "Timer.h"
#include "TransformComponent.h"


WrongWayManager::WrongWayManager(int maxNbr, float spawnInterval)
	:EnemyManager(maxNbr, spawnInterval)
{

}

WrongWayManager::WrongWayManager(WrongWayManager const& other)
	: EnemyManager(other)
{

}
void WrongWayManager::SpawnerTimer()
{
	if (m_NbrEnemies < MAX_ENEMY_OF_TYPE)
	{
		if (m_EnemySpawnTimer < SPAWN_INTERVAL)
		{
			m_EnemySpawnTimer += empire::Timer::GetInstance().GetDeltaTime();
		}
		else
		{
			int random{ rand() % 2 };

			GameObject* pWrongWay = PrefabsManager::GetInstance().Instantiate("WrongWay");
			auto pWWComp = pWrongWay->GetComponent<WrongWay>();
			if (random == 0)
			{
				pWWComp->SetCurrentQube(m_pPyramid->GetEscheresqueRightTop());
				pWWComp->SetEscheresqueRight(true);
				pWrongWay->GetTransform()->SetWorldPosition(pWWComp->GetCurrentQube()->GetEscheresqueRightPos());
			}
			else
			{
				pWWComp->SetCurrentQube(m_pPyramid->GetEscheresqueLeftTop());
				pWWComp->SetEscheresqueRight(false);
				pWrongWay->GetTransform()->SetWorldPosition(pWWComp->GetCurrentQube()->GetEscheresqueLeftPos());
			}

			AddToArray(pWrongWay->GetComponent<WrongWay>());
			m_pPyramid->GetGameObject()->AddChild(pWrongWay);
			if (m_pObserver != nullptr)
			{
				pWrongWay->AddObserver(m_pObserver);
			}
			m_EnemySpawnTimer = 0;
			m_NbrEnemies++;
		}
	}
}