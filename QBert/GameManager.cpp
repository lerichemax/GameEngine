#include "PCH.h"
#include "GameManager.h"

#include "EnemyManager.h"
#include "QBert.h"
#include "Coily.h"
#include "SlickSam.h"
#include "WrongWay.h"
#include "CharacterPoint.h"
#include "CharacterLives.h"
#include "InputManager.h"

#include "PauseGameCommand.h"

#include "TextRendererComponent.h"
#include "RendererComponent.h"
#include "Timer.h"



#include "CoilyManager.h"
#include "ColoredDisk.h"
#include "Pyramid.h"
#include "WrongWayManager.h"
#include "SlickSamManager.h"

#include <algorithm>

//EventHandler<GameManagerBehaviour, bool> GameManagerBehaviour::OnGamePaused{};

GameManager::GameManager(TextRendererComponent* pP1Points, TextRendererComponent* pP2Points,
                         TextRendererComponent* pP1Lives, TextRendererComponent* pP2Lives,
                         CoilyManager* pCm, WrongWayManager* pWWm, SlickSamManager* pSSm, GameObject* pGameOver, 
						unsigned int nbrPlayers)
	:m_pP1PointsCounter(pP1Points),
	m_P1LivesCounter(pP1Lives),
	m_pP2PointsCounter(pP2Points),
	m_P2LivesCounter(pP2Lives),
	m_pCManager(pCm),
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

		if (m_pCManager !=nullptr) m_pCManager->Reset();
		if (m_pWWManager != nullptr) m_pWWManager->Reset();
		if (m_pSSManager != nullptr) m_pSSManager->Reset();
			
		//pPlayer->SetCurrentQube(pPlayer->GetCurrentQube());
		//pPlayer->GetGameObject()->GetComponent<RendererComponent>()->ChangeLayer(Layer::foreground);
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
			Debugger::GetInstance().Log("GAME OVER");
			m_pGameOver->GetComponentInChildren<TextRendererComponent>()->SetText("Game Over");
			Timer::GetInstance().SetTimeScale(0);
			m_pGameOver->SetActive(true);
			m_NbrDeadPlayers = 0;
		}
		break;
	}
	case GameEvent::JumpOnDisk:
		if (m_pCManager != nullptr) m_pCManager->SetIdle(true);
		
		break;
	case GameEvent::JumpOffDisk:
		if (m_pCManager != nullptr) m_pCManager->SetIdle(false);
		break;
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
		//pPyramid->GetQBert()->EarnPoints(pPyramid->GetNbrDisks() * ColoredDisk::GetPoints());
		break;
	}
}

void GameManager::UpdateLivesText(CharacterLives* pLives, int playerNbr)
{
	switch (playerNbr)
	{
	case 1:
		//m_P1LivesCounter->SetText("P1 Lives: " + std::to_string(pLives->GetNbrLives()));
		break;
	case 2:
		//m_P2LivesCounter->SetText("P2 Lives: " + std::to_string(pLives->GetNbrLives()));
		break;
	}
}

void GameManager::UpdatePointsText(CharacterPoint* pPoint, int playerNbr)
{
	switch (playerNbr)
	{
	case 1:
		//m_pP1PointsCounter->SetText("P1 Points: " + std::to_string(pPoint->GetPoints()));
		break;
	case 2:
		//m_pP2PointsCounter->SetText("P2 Points: " + std::to_string(pPoint->GetPoints()));
		break;
	}
}

GameManagerBehaviour::GameManagerBehaviour()
	:m_IsPaused{false}
{
}

void GameManagerBehaviour::Start()
{
	m_Level = Level::Level1;

	m_pPyramid = FindComponentOfType<PyramidSystem>();
	m_pQbert = FindComponentOfType<QBert>();

	if (m_pPyramid != nullptr)
	{
		m_pPyramid->OnAllQubesFlipped.Subscribe([this](int) {
			HandleEndGame();
			});
	}

	if (m_pQbert != nullptr)
	{
		m_pQbert->GetGameObject()->GetComponent<CharacterLives>()->OnGameOver.Subscribe([this]() {
			Timer::GetInstance().SetTimeScale(0);
			});
	}

	//InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_p , KeyActionState::released,
	//new PauseGameCommand(GetGameObject()->GetComponent<GameManagerBehaviour>()) });
}

void GameManagerBehaviour::Update()
{
	if (InputManager::GetInstance().IsUp(SDL_SCANCODE_P))
	{
		TogglePause();
	}
}

void GameManagerBehaviour::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(GameManagerBehaviour).name());

	BehaviourComponent::Serialize(writer);
}

void GameManagerBehaviour::TogglePause()
{
	Timer::GetInstance().SetTimeScale(m_IsPaused ? 1.f : 0.f);

	m_IsPaused = !m_IsPaused;
	OnGamePaused.Notify(m_IsPaused);
}

void GameManagerBehaviour::ResetGame()
{
	m_pPyramid->Reset(m_Level);
	m_pQbert->Reset(false, m_pPyramid->GetTop());
}

void GameManagerBehaviour::HandleEndGame()
{
	switch (m_Level)
	{
	case Level::Level1:
		Debugger::GetInstance().Log("YOU FINISHED LEVEL 1!");
		m_Level = Level::Level2;
		ResetGame();
		break;
	case Level::Level2:
		Debugger::GetInstance().Log("YOU FINISHED LEVEL 2!");
		m_Level = Level::Level3;
		ResetGame();
		break;
	case Level::Level3:
		Debugger::GetInstance().Log("YOU FINISHED LEVEL 3!");
		OnGameEnded.Notify();
		break;
	default:
		break;
	}
}