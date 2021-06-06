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
void WrongWayManager::Spawn()
{
	int const random{ rand() % 2 };

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
	AddToArray(pWWComp);
	
	m_pPyramid->GetGameObject()->AddChild(pWrongWay);
	if (m_pObserver != nullptr)
	{
		pWrongWay->AddObserver(m_pObserver);
	}
}