#include "PCH.h"
#include "GameManagerSystem.h"
#include "QBertSystem.h"
#include "PyramidSystem.h"
#include "LivesSystem.h"
#include "EnemySpawnerSystem.h"

#include "TextRendererComponent.h"
#include "RendererComponent.h"
#include "CharacterPoint.h"
#include "CharacterLives.h"
#include "QbertComponent.h"

#include "Timer.h"
#include "InputManager.h"

#include <algorithm>

GameManager::GameManager(CoilyManager* pCm, WrongWayManager* pWWm, SlickSamManager* pSSm, GameObject* pGameOver, 
						unsigned int nbrPlayers)
	:m_pCManager(pCm),
	m_pSSManager(pSSm),
	m_pWWManager(pWWm),
	m_pGameOver(pGameOver),
	m_NbrPlayers(nbrPlayers),
	m_NbrDeadPlayers()
{
}

void GameManager::Notify(GameObject* object, int event)
{
	switch (static_cast<GameEvent>(event))
	{
	case GameEvent::PlayerDied:
	{
		//auto pPlayer = object->GetComponent<QBert>();
			
		//UpdateLivesText(object->GetComponent<CharacterLives>(), pPlayer->GetPlayerNbr());

		//if (m_pCManager !=nullptr) m_pCManager->Reset();
		//if (m_pWWManager != nullptr) m_pWWManager->Reset();
		//if (m_pSSManager != nullptr) m_pSSManager->Reset();
		//	
		//pPlayer->SetCurrentQube(pPlayer->GetCurrentQube());
		//pPlayer->GetEntity()->GetComponent<RendererComponent>()->ChangeLayer(Layer::foreground);
		//pPlayer->SetCanMove();
		
		break;
	}
	case GameEvent::IncreasePoints:
	{
		//auto const pPlayer = object->GetComponent<QBert>();
		//UpdatePointsText(object->GetComponent<CharacterPoint>(), pPlayer->GetPlayerNbr());
	}
	break;
	case GameEvent::GameOver:
	{
		//auto pPlayer = object->GetComponent<QBert>();

		//UpdateLivesText(object->GetComponent<CharacterLives>(), pPlayer->GetPlayerNbr());
		object->SetActive(false);
		m_NbrDeadPlayers++;
		if (m_NbrDeadPlayers >= m_NbrPlayers)
		{
			Debugger::Get().Log("GAME OVER");
			m_pGameOver->GetComponentInChildren<TextRendererComponent>()->SetText("Game Over");
			Timer::Get().SetTimeScale(0);
			m_pGameOver->SetActive(true);
			m_NbrDeadPlayers = 0;
		}
		break;
	}
	//case GameEvent::JumpOnDisk:
	//	if (m_pCManager != nullptr) m_pCManager->SetIdle(true);
	//	
	//	break;
	//case GameEvent::JumpOffDisk:
	//	if (m_pCManager != nullptr) m_pCManager->SetIdle(false);
	//	break;
	//case GameEvent::CoilyDies:
	//	if (m_pCManager != nullptr) m_pCManager->EnemyDied(object->GetComponent<Coily>());
	//	break;
	//case GameEvent::SlickSamDies:
	//	if (m_pSSManager != nullptr) m_pSSManager->EnemyDied(object->GetComponent<SlickSam>());
	//	break;
	//case GameEvent::WrongWayDies:
	//	if (m_pWWManager != nullptr) m_pWWManagergma->EnemyDied(object->GetComponent<WrongWay>());
	//	break;
	case GameEvent::PyramidCompleted:
		//auto const pPyramid = object->GetComponent<Pyramid>();
		//pPyramid->GetQBert()->EarnPoints(pPyramid->GetNbrDisks() * DiskSystem::GetPoints());
		break;
	}
}

GameManagerSystem::GameManagerSystem()
	:m_IsPaused{false}
{
}

void GameManagerSystem::Start()
{
	m_Level = Level::Level1;

	m_pPyramid = m_pRegistry->GetSystem<PyramidSystem>();
	m_pQbert = m_pRegistry->GetSystem<QBertSystem>();

	m_pQbert->SetStartQubes(m_GameMode);

	m_pRegistry->GetSystem<LivesSystem>()->OnDied.Subscribe([this](Entity deadEntity, int) {
		if (m_pRegistry->HasTag(deadEntity, QBERT_TAG))
		{
			if (m_GameMode == GameMode::Normal)
			{
				m_pRegistry->GetSystem<EnemySpawnerSystem>()->Reset();
			}
		}
	});

	if (m_pPyramid != nullptr)
	{
		m_pPyramid->OnAllQubesFlipped.Subscribe([this](int) {
			HandleEndGame();
			});
	}

	m_pRegistry->GetSystem<LivesSystem>()->OnPlayerDied.Subscribe([this]() {
		if (m_GameMode == GameMode::Coop)
		{
			m_NbrPlayerDead++;
			if (m_NbrPlayerDead < 2)
			{
				return;
			}
		}

		Timer::Get().SetTimeScale(0);
		OnGameOver.Notify();
	});

}

void GameManagerSystem::Update()
{
	if (InputManager::Get().IsUp(SDL_SCANCODE_P) || InputManager::Get().IsUp(SDL_SCANCODE_ESCAPE))
	{
		TogglePause();
	}
}

void GameManagerSystem::SetSignature() const
{
	Signature signature{};

	m_pRegistry->SetSystemSignature<GameManagerSystem>(signature);
}

void GameManagerSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(GameManagerSystem).name());

	System::Serialize(writer);
}

void GameManagerSystem::TogglePause()
{
	Timer::Get().SetTimeScale(m_IsPaused ? 1.f : 0.f);

	m_IsPaused = !m_IsPaused;
	OnGamePaused.Notify(m_IsPaused);
}

void GameManagerSystem::SetGameMode(GameMode mode)
{
	m_GameMode = mode;
}

void GameManagerSystem::ResetGame()
{
	m_pPyramid->Reset(m_Level);
	m_pQbert->Reset(m_GameMode);
	m_NbrPlayerDead = 0;
}

void GameManagerSystem::HandleEndGame()
{
	switch (m_Level)
	{
	case Level::Level1:
		Debugger::Get().Log("YOU FINISHED LEVEL 1!");
		m_Level = Level::Level2;
		ResetGame();
		break;
	case Level::Level2:
		Debugger::Get().Log("YOU FINISHED LEVEL 2!");
		m_Level = Level::Level3;
		ResetGame();
		break;
	case Level::Level3:
		Debugger::Get().Log("YOU FINISHED LEVEL 3!");
		OnGameEnded.Notify();
		Timer::Get().SetTimeScale(0.f);
		break;
	default:
		break;
	}
}