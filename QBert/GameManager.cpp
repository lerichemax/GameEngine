#include "PCH.h"
#include "GameManager.h"
#include "EnemyManager.h"
#include "QBert.h"
#include "QBertScene.h"
#include "Coily.h"
#include "SlickSam.h"
#include "WrongWay.h"
#include "CharacterPoint.h"
#include "CharacterLives.h"

#include "TextRendererComponent.h"
#include "RendererComponent.h"
#include "Timer.h"

GameManager::GameManager(empire::TextRendererComponent* pP1Points, empire::TextRendererComponent* pP2Points,
                         empire::TextRendererComponent* pP1Lives, empire::TextRendererComponent* pP2Lives, EnemyManager* pManager,
                         GameObject* pGameOver, unsigned int nbrPlayers)
	:m_pP1PointsCounter(pP1Points),
	m_P1LivesCounter(pP1Lives),
	m_pP2PointsCounter(pP2Points),
	m_P2LivesCounter(pP2Lives),
	m_pEnemyManager(pManager),
	m_NbrPlayers(nbrPlayers),
	m_NbrDeadPlayers(),
	m_pGameOver(pGameOver)
{
}

void GameManager::Notify(empire::GameObject* object, int event)
{
	switch ((GameEvent)event)
	{
	case GameEvent::PlayerDied:
	{
		auto pPlayer = object->GetComponent<QBert>();
		UpdateLivesText(object->GetComponent<CharacterLives>(), pPlayer->GetPlayerNbr());
		
		pPlayer->SetCurrentQube(pPlayer->GetCurrentQube());
		pPlayer->GetGameObject()->GetComponent<empire::RendererComponent>()->ChangeLayer(empire::Layer::foreground);
		pPlayer->SetCanMove();
		m_pEnemyManager->Reset();
		break;
	}
	case GameEvent::IncreasePoints:
	{
		auto pPlayer = object->GetComponent<QBert>();
		UpdatePointsText(object->GetComponent<CharacterPoint>(), pPlayer->GetPlayerNbr());
	}
	break;
	case GameEvent::GameOver:
	{
		auto pPlayer = object->GetComponent<QBert>();
		UpdateLivesText(object->GetComponent<CharacterLives>(), pPlayer->GetPlayerNbr());
		object->SetActive(false);
		m_NbrDeadPlayers++;
		if (m_NbrDeadPlayers >= m_NbrPlayers)
		{
			empire::Debugger::GetInstance().Log("GAME OVER");
			m_pGameOver->GetComponentInChildren<TextRendererComponent>()->SetText("Game Over");
			Timer::GetInstance().SetTimeScale(0);
			m_pGameOver->SetActive(true);
			//static_cast<QBertScene*>(object->GetParentScene())->ResetGame();
		}
		break;
	}
	case GameEvent::JumpOnDisk:
		m_pEnemyManager->SetCoiliesIdle(true);
		break;
	case GameEvent::JumpOffDisk:
		m_pEnemyManager->SetCoiliesIdle(false);
		break;
	case GameEvent::CoilyDies:
		m_pEnemyManager->CoilyDied(object->GetComponent<Coily>());
		break;
	case GameEvent::SlickSamDies:
		m_pEnemyManager->SlickSamDied(object->GetComponent<SlickSam>());
		break;
	case GameEvent::WrongWayDies:
		m_pEnemyManager->WrongWayDied(object->GetComponent<WrongWay>());
		break;
	}
}

void GameManager::UpdateLivesText(CharacterLives* pLives, int playerNbr)
{
	switch (playerNbr)
	{
	case 1:
		m_P1LivesCounter->SetText("P1 Lives: " + std::to_string(pLives->GetNbrLives()));
		break;
	case 2:
		m_P2LivesCounter->SetText("P2 Lives: " + std::to_string(pLives->GetNbrLives()));
		break;
	}
}

void GameManager::UpdatePointsText(CharacterPoint* pPoint, int playerNbr)
{
	switch (playerNbr)
	{
	case 1:
		m_pP1PointsCounter->SetText("P1 Points: " + std::to_string(pPoint->GetPoints()));
		break;
	case 2:
		m_pP2PointsCounter->SetText("P2 Points: " + std::to_string(pPoint->GetPoints()));
		break;
	}
}