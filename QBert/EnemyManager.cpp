#include "PCH.h"
#include "EnemyManager.h"
#include "Coily.h"
#include "SlickSam.h"
#include "Pyramid.h"
#include "WrongWay.h"
#include "GameManager.h"
#include "CoilyCharacterController.h"

#include "Timer.h"
#include "ObserverManager.h"
#include "PrefabsManager.h"


EnemyManager::EnemyManager()
	: m_pPyramid(nullptr),
	m_pCoilies{},
	m_pSlickSams{}
{
	
}

void EnemyManager::Initialize()
{
	m_pCoilies.fill(nullptr);
	m_pSlickSams.fill(nullptr);
	m_pWrongWays.fill(nullptr);
}

void EnemyManager::Update()
{
	if (m_pPyramid == nullptr)
	{
		Debugger::GetInstance().LogError("Variable m_pPyramid is undefined In EnemyManager");
	}
	//CoilySpawnerTimer();
	//SlickSamSpawnerTimer();
	//WrongWaySpawnerTimer();
}

void EnemyManager::CoilySpawnerTimer()
{
	//Spawn Coilies
	if (m_NbrCoily < MAX_COILY)
	{
		if (m_CoilySpawnTimer < COILY_SPAWN_INTERVAL)
		{
			m_CoilySpawnTimer += empire::Timer::GetInstance().GetDeltaTime();
			return;
		}

		int random{ rand() % 2 + 1 };
		GameObject* pCoily = PrefabsManager::GetInstance().Instantiate("Coily");
		pCoily->GetComponent<CoilyCharacterController>()->SetPyramid(m_pPyramid);
		pCoily->GetComponent<Coily>()->SetQube(m_pPyramid->GetQubes()[random]);

		AddCoilyToArray(pCoily->GetComponent<Coily>());
		pCoily->AddObserver(empire::ObserverManager::GetInstance().GetObserver<GameManager>());
		m_CoilySpawnTimer = 0;
		m_NbrCoily++;
		m_pPyramid->GetGameObject()->AddChild(pCoily);
	}
}

void EnemyManager::AddCoilyToArray(Coily* pCoily)
{
	for (int i = 0; i < MAX_COILY; i++)
	{
		if (m_pCoilies[i] == nullptr)
		{
			m_pCoilies[i] = pCoily;
			return;
		}
	}
}

void EnemyManager::SlickSamSpawnerTimer()
{
	//Spawn Slick and Sam
	if (m_NbrSlickSam < MAX_SLICKSAM)
	{
		if (m_SlickSamSpawnTimer < SLICKSAM_SPAWN_INTERVAL)
		{
			m_SlickSamSpawnTimer += empire::Timer::GetInstance().GetDeltaTime();
		}
		else
		{
			int random{ rand() % 2 + 1 };
			GameObject* pSlickSam = PrefabsManager::GetInstance().Instantiate("SlickSam");
			pSlickSam->GetComponent<SlickSam>()->SetQube(m_pPyramid->GetQubes()[random]);
			
			AddSlickSamToArray(pSlickSam->GetComponent<SlickSam>());
			m_pPyramid->GetGameObject()->AddChild(pSlickSam);
			pSlickSam->AddObserver(ObserverManager::GetInstance().GetObserver<GameManager>());
			m_SlickSamSpawnTimer = 0;
			m_NbrSlickSam++;
		}
	}
}

void EnemyManager::WrongWaySpawnerTimer()
{
	if (m_NbrWrongWay < MAX_WRONGWAY)
	{
		if (m_WrongWaySpawnTimer < WRONGWAY_SPAWN_INTERVAL)
		{
			m_WrongWaySpawnTimer += empire::Timer::GetInstance().GetDeltaTime();
		}
		else
		{
			int random{ rand() % 2 };
			
			GameObject* pWrongWay = PrefabsManager::GetInstance().Instantiate("WrongWay");
			if (random == 0)
			{
				pWrongWay->GetComponent<WrongWay>()->SetQube(m_pPyramid->GetEscheresqueRightTop());
				pWrongWay->GetComponent<WrongWay>()->SetEscheresqueRight(true);
			}
			else
			{
				pWrongWay->GetComponent<WrongWay>()->SetQube(m_pPyramid->GetEscheresqueLeftTop());
				pWrongWay->GetComponent<WrongWay>()->SetEscheresqueRight(false);
			}
			
			AddWrongWayToArray(pWrongWay->GetComponent<WrongWay>());
			m_pPyramid->GetGameObject()->AddChild(pWrongWay);
			pWrongWay->AddObserver(ObserverManager::GetInstance().GetObserver<GameManager>());
			m_WrongWaySpawnTimer = 0;
			m_NbrWrongWay++;
		}
	}
}

void EnemyManager::AddSlickSamToArray(SlickSam* pSlickSam)
{
	for (unsigned int i = 0; i < MAX_SLICKSAM; i++)
	{
		if (m_pSlickSams[i] == nullptr)
		{
			m_pSlickSams[i] = pSlickSam;
			return;
		}
	}
}

void EnemyManager::AddWrongWayToArray(WrongWay* pWrongWay)
{
	for (unsigned int i = 0; i < MAX_WRONGWAY; i++)
	{
		if (m_pWrongWays[i] == nullptr)
		{
			m_pWrongWays[i] = pWrongWay;
			return;
		}
	}
}

void EnemyManager::CoilyDied(Coily* pCoily)
{
	for (unsigned int i{}; i < MAX_COILY; i++)
	{
		if (m_pCoilies[i] == pCoily)
		{
			m_pCoilies[i] = nullptr;
			break;
		}
	}
	m_NbrCoily--;
}

void EnemyManager::SlickSamDied(SlickSam* pSlickSam)
{
	for (int i{}; i < MAX_SLICKSAM; i++)
	{
		if (m_pSlickSams[i] == pSlickSam)
		{
			m_pSlickSams[i] = nullptr;
			break;
		}
	}
	m_NbrSlickSam--;
}

void EnemyManager::WrongWayDied(WrongWay* pWrongWay)
{
	for (int i{}; i < MAX_WRONGWAY; i++)
	{
		if (m_pWrongWays[i] == pWrongWay)
		{
			m_pWrongWays[i] = nullptr;
			break;
		}
	}
	m_NbrWrongWay--;
}

void EnemyManager::Reset()
{
	for (auto pCoily : m_pCoilies)
	{
		if (pCoily != nullptr)
		{
			pCoily->Die();
		}
	}
	m_NbrCoily = 0;

	for (auto pSlickSam : m_pSlickSams)
	{
		if (pSlickSam != nullptr)
		{
			pSlickSam->Die();
		}
	}
	m_NbrSlickSam = 0;

	for (auto pWrongWay : m_pWrongWays)
	{
		if (pWrongWay != nullptr)
		{
			pWrongWay->Die();
		}
	}
	m_NbrWrongWay = 0;
}

void EnemyManager::ResetTimers()
{
	m_CoilySpawnTimer = 0;
	m_SlickSamSpawnTimer = 0;
	m_WrongWaySpawnTimer = 0;
}

void EnemyManager::SetCoiliesIdle(bool isIdle)
{
	for (auto pCoily : m_pCoilies)
	{
		if (pCoily != nullptr)
		{
			pCoily->GetGameObject()->GetComponent<CoilyCharacterController>()->SetIdle(isIdle);
		}
	}
}