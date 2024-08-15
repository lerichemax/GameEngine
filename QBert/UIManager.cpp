#include "PCH.h"
#include "UIManager.h"

#include "CharacterPoint.h"
#include "CharacterLives.h"
#include "TextRendererComponent.h"
#include "GameManager.h"

void UIManager::Start()
{
	FindComponentOfType<ECS_CharacterPoint>()->OnPointsUpdated.Subscribe([this](int points) {
		m_pP1PointsCounter->m_Text = "P1 Points: " + std::to_string(points);
		});

	auto characterLives = FindComponentOfType<ECS_CharacterLives>();
	characterLives->OnDied.Subscribe([this](int lives) {
		m_P1LivesCounter->m_Text = "P1 Lives: " + std::to_string(lives);
		});

	characterLives->OnGameOver.Subscribe([this]() {
		m_pGameOverMenu->GetComponentInChildren<ECS_TextRendererComponent>()->m_Text = "Game Over";
		m_pGameOverMenu->SetActive(true);
		});

	FindComponentOfType<GameManagerBehaviour>()->OnGamePaused.Subscribe([this](bool isPaused) {
		m_pPauseMenu->SetActive(isPaused);
		});

	FindComponentOfType<GameManagerBehaviour>()->OnGameEnded.Subscribe([this]() {
		m_pGameOverMenu->GetComponentInChildren<ECS_TextRendererComponent>()->m_Text = "You Win";
		m_pGameOverMenu->SetActive(true);
	});
}

void UIManager::SetP1PointsCounter(std::shared_ptr<ECS_TextRendererComponent> p1Points)
{
	m_pP1PointsCounter = p1Points;
}

void UIManager::SetP1LivesCounter(std::shared_ptr<ECS_TextRendererComponent> p1Lives)
{
	m_P1LivesCounter = p1Lives;
}

void UIManager::SetPauseMenu(std::shared_ptr<GameObject> pauseMenu)
{
	m_pPauseMenu = pauseMenu;
}

void UIManager::SetGameOverMenu(std::shared_ptr<GameObject> gameOverMenu)
{
	m_pGameOverMenu = gameOverMenu;
}