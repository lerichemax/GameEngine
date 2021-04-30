#include "PCH.h"
#include "EnemyManager.h"
#include "Coily.h"
#include "SlickSam.h"
#include "Pyramid.h"

#include "Timer.h"
#include "ObserverManager.h"
#include "PrefabsManager.h"

EnemyManager::EnemyManager(Pyramid* const pPyramid)
	: m_pPyramid(pPyramid),
	m_pCoilies{},
	m_pSlickSams{}
{
	m_pCoilies.fill(nullptr);
	m_pSlickSams.fill(nullptr);
}

void EnemyManager::Update()
{
	CoilySpawnerTimer();
	SlickSamSpawnerTimer();
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
		pCoily->GetComponent<Coily>()->SetQube(m_pPyramid->GetQubes()[random]);

		AddCoilyToArray(pCoily->GetComponent<Coily>());
		pCoily->GetComponent<Coily>()->GetSubject()->AddObserver(empire::ObserverManager::GetInstance().GetObserver(30));
		m_CoilySpawnTimer = 0;
		m_NbrCoily++;
		m_pPyramid->GetGameObject()->AddChild(pCoily);
		std::cout << "Coily spawned\n";
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
			pSlickSam->GetComponent<SlickSam>()->GetSubject()->AddObserver(ObserverManager::GetInstance().GetObserver(30));
			m_SlickSamSpawnTimer = 0;
			m_NbrSlickSam++;
			std::cout << "Slick Sam spawned\n";
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

void EnemyManager::Reset()
{
	for (auto pCoily : m_pCoilies)
	{
		if (pCoily != nullptr)
		{
			pCoily->Die();
			pCoily = nullptr;
		}
		
	}
	m_NbrCoily = 0;

	for (auto pSlickSam : m_pSlickSams)
	{
		if (pSlickSam != nullptr)
		{
			pSlickSam->Die();
			pSlickSam = nullptr;
		}
	}
	m_NbrSlickSam = 0;
}

void EnemyManager::SetCoiliesIdle(bool isIdle)
{
	for (auto pCoily : m_pCoilies)
	{
		if (pCoily != nullptr)
		{
			pCoily->SetIsIdle(isIdle);
		}
	}
}