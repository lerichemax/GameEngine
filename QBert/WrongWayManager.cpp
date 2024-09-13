#include "PCH.h"
#include "WrongWayManager.h"

#include "GameManagerSystem.h"
#include "WrongWay.h"

#include "ObserverManager.h"
#include "PrefabsManager.h"

#include "Timer.h"
#include "TransformComponent.h"


void WrongWayManager::Spawn()
{
	int const random{ rand() % 2 };

	//GameObject* pWrongWay = PrefabsManager::GetInstance().Instantiate("WrongWay");
	//auto pWWComp = pWrongWay->GetComponent<WrongWay>();
	if (random == 0)
	{
		//pWWComp->SetCurrentQube(m_pPyramid->GetEscheresqueRightTop());
		//pWWComp->SetEscheresqueRight(true);
		//pWrongWay->GetECSTransform()->Translate(pWWComp->GetCurrentQube()->GetEscheresqueRightPos());
		//pWrongWay->GetECSTransform()->SetWorldPosition(pWWComp->GetCurrentQube()->GetEscheresqueRightPos());
	}
	else
	{
		//pWWComp->SetCurrentQube(m_pPyramid->GetEscheresqueLeftTop());
		//pWWComp->SetEscheresqueRight(false);
		//pWrongWay->GetECSTransform()->Translate(pWWComp->GetCurrentQube()->GetEscheresqueLeftPos());
		//pWrongWay->GetECSTransform()->SetWorldPosition(pWWComp->GetCurrentQube()->GetEscheresqueLeftPos());
	}
	//AddToArray(pWWComp);
	
	//m_pPyramid->GetGameObject()->AddChild(pWrongWay);
	//if (m_pObserver != nullptr)
	//{
	//	pWrongWay->AddObserver(m_pObserver);
	//}
}