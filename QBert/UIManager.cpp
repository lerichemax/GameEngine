#include "PCH.h"
#include "UIManager.h"

#include "CharacterPoint.h"
#include "CharacterLives.h"
#include "TextRendererComponent.h"
#include "GameManager.h"

void UIManager::Start()
{
	FindComponentOfType<CharacterPoint>()->OnPointsUpdated.Subscribe([this](int points) {
		m_pP1PointsCounter->SetText("P1 Points: " + std::to_string(points));
		});

	auto characterLives = FindComponentOfType<CharacterLives>();
	characterLives->OnDied.Subscribe([this](int lives) {
		m_P1LivesCounter->SetText("P1 Lives: " + std::to_string(lives));
		});

	characterLives->OnGameOver.Subscribe([this]() {
		m_pGameOverMenu->GetComponentInChildren<TextRendererComponent>()->SetText("Game Over");
		m_pGameOverMenu->SetActive(true);
		});

	FindComponentOfType<GameManagerBehaviour>()->OnGamePaused.Subscribe([this](bool isPaused) {
		m_pPauseMenu->SetActive(isPaused);
		});

	FindComponentOfType<GameManagerBehaviour>()->OnGameEnded.Subscribe([this]() {
		m_pGameOverMenu->GetComponentInChildren<TextRendererComponent>()->SetText("You Win");
		m_pGameOverMenu->SetActive(true);
	});
}

void UIManager::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(UIManager).name());

	BehaviourComponent::Serialize(writer);
}

void UIManager::SetP1PointsCounter(TextRendererComponent* const p1Points)
{
	m_pP1PointsCounter = p1Points;
}

void UIManager::SetP1LivesCounter(TextRendererComponent* const p1Lives)
{
	m_P1LivesCounter = p1Lives;
}

void UIManager::SetPauseMenu(GameObject* const pauseMenu)
{
	m_pPauseMenu = pauseMenu;
}

void UIManager::SetGameOverMenu(GameObject* const gameOverMenu)
{
	m_pGameOverMenu = gameOverMenu;
}