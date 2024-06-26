#include "PCH.h"
#include "CoilyManager.h"

#include "GameManager.h"
#include "Coily.h"
#include "Pyramid.h"
#include "CoilyCharacterController.h"

#include "ObserverManager.h"
#include "PrefabsManager.h"
#include "Qube.h"
#include "Timer.h"
#include "TransformComponent.h"


CoilyManager::CoilyManager(int maxNbr, float spawnInterval)
	:EnemyManager(maxNbr, spawnInterval)
{

}

CoilyManager::CoilyManager(CoilyManager const& other)
	: EnemyManager(other)
{

}

void CoilyManager::Spawn()
{
	//int const random{ rand() % 2 + 1 };
	//GameObject* pCoily = PrefabsManager::GetInstance().Instantiate("Coily");
	//m_pPyramid->GetGameObject()->AddChild(pCoily);
	//pCoily->GetComponent<CoilyCharacterController>()->SetPyramid(m_pPyramid);
	//pCoily->GetComponent<Coily>()->SetCurrentQube(m_pPyramid->GetQubes()[random]);
	//pCoily->GetECSTransform()->SetWorldPosition(pCoily->GetComponent<Coily>()->GetCurrentQube()->GetCharacterPos());
	//pCoily->GetECSTransform()->Translate(pCoily->GetComponent<Coily>()->GetCurrentQube()->GetCharacterPos());

	//AddToArray(pCoily->GetComponent<Coily>());
	//if (m_pObserver != nullptr)
	//{
	//	pCoily->AddObserver(m_pObserver);
	//}
}

void CoilyManager::SetIdle(bool isIdle)
{
	for (size_t i{}; i < MAX_ENEMY_OF_TYPE;++i)
	{
		if (m_pEnemies[i] != nullptr && isIdle)
		{
			//m_pEnemies[i]->GetGameObject()->GetComponent<CoilyCharacterController>()->SetIdle(isIdle);
		}
	}
}
