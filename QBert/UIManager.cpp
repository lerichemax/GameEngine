#include "PCH.h"
#include "UIManager.h"

#include "CharacterPoint.h"
#include "CharacterLives.h"
#include "TextRendererComponent.h"

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
		m_pGameOverMenu->SetActive(true);
		});
}