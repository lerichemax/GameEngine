#include "PCH.h"
#include "SlickSamManager.h"

#include "GameManager.h"
#include "SlickSam.h"
#include "Pyramid.h"
#include "Qube.h"
#include "TransformComponent.h"

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
void SlickSamManager::Spawn()
{
	int const random{ rand() % 2 + 1 };
	GameObject* pSlickSam = PrefabsManager::GetInstance().Instantiate("SlickSam");

	m_pPyramid->GetGameObject()->AddChild(pSlickSam);

	pSlickSam->GetComponent<SlickSam>()->SetCurrentQube(m_pPyramid->GetQubes()[random]);
	//pSlickSam->GetECSTransform()->SetWorldPosition(pSlickSam->GetComponent<SlickSam>()->GetCurrentQube()->GetCharacterPos());
	pSlickSam->GetECSTransform()->Translate(pSlickSam->GetComponent<SlickSam>()->GetCurrentQube()->GetCharacterPos());
	AddToArray(pSlickSam->GetComponent<SlickSam>());
	if (m_pObserver != nullptr)
	{
		pSlickSam->AddObserver(m_pObserver);
	}
}