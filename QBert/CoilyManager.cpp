#include "PCH.h"
#include "CoilyManager.h"

#include "GameManagerSystem.h"
#include "Coily.h"
#include "CoilyCharacterController.h"


#include "PrefabsManager.h"
#include "Timer.h"
#include "TransformComponent.h"


void CoilyManager::Spawn()
{
	//int const random{ rand() % 2 + 1 };
	//GameObject* pCoily = PrefabsManager::Get().Instantiate("Coily");
	//m_pPyramid->GetEntity()->AddChild(pCoily);
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
	//for (size_t i{}; i < MAX_ENEMY_OF_TYPE;++i)
	//{
	//	if (m_pEnemies[i] != nullptr && isIdle)
	//	{
	//		//m_pEnemies[i]->GetEntity()->GetComponent<CoilyCharacterController>()->SetIdle(isIdle);
	//	}
	//}
}
