#include "PCH.h"
#include "GameManagerSystem.h"
#include "QBertSystem.h"
#include "PyramidSystem.h"
#include "LivesSystem.h"
#include "EnemySpawnerSystem.h"
#include "CoilySystem.h"
#include "QubeSystem.h"

#include "TextRendererComponent.h"
#include "RendererComponent.h"
#include "CharacterPoint.h"
#include "CharacterLives.h"
#include "QbertComponent.h"
#include "CoilyComponent.h"
#include "AiControllerComponent.h"
#include "CharacterControllerComponent.h"
#include "MovementComponent.h"
#include "QubeComponent.h"

#include "Timer.h"
#include "InputManager.h"

#include <algorithm>

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

	m_pPyramid->OnAllQubesFlipped.Subscribe([this](int) {
		if (m_GameMode == GameMode::Versus)
		{
			HandleEndRound(true);
		}
		else
		{
			HandleEndGame();
		}
		});


	SubscribeLifeSystem();

	if (m_GameMode == GameMode::Versus)
	{
		m_pRegistry->GetSystem<CoilySystem>()->OnCoilyTransformed.Subscribe([this](Entity coilyEntity) {
			m_pRegistry->GetComponent<CoilyComponent>(coilyEntity)->SetActive(false);
			m_pRegistry->GetComponent<AiControllerComponent>(coilyEntity)->SetActive(false);
			m_pRegistry->GetComponent<CharacterControllerComponent>(coilyEntity)->SetActive(true);
		});

		Entity coilyEntity = m_pRegistry->FindComponentOfType<CoilyComponent>()->GetEntity();
		m_pRegistry->GetComponent<MovementComponent>(coilyEntity)->CurrentQube = m_pPyramid->GetQubeAtIndex(2);

		m_pRegistry->GetComponent<CharacterPoint>(coilyEntity)->OnPointsUpdated.Subscribe([this](int points, Entity entity) {
			if (points >= 3)
			{
				OnGameEnded.Notify("Player 2 wins");
			}
		});

		Entity qBertEntity = m_pRegistry->FindComponentOfType<QbertComponent>()->GetEntity();
		m_pRegistry->GetComponent<CharacterPoint>(qBertEntity)->OnPointsUpdated.Subscribe([this](int points, Entity entity) {
			if (points >= 3)
			{
				OnGameEnded.Notify("Player 1 wins");
			}
			});
	}
	else
	{
		QubeSystem::OnAnyQubeFlipped.Subscribe([this]() {
			m_pQbert->AddPoints(QubeComponent::POINTS_FOR_FLIP);
			});
	}
}

void GameManagerSystem::SubscribeLifeSystem()
{
	auto* const pLivesSystem = m_pRegistry->GetSystem<LivesSystem>();

	pLivesSystem->OnDied.Subscribe([this](Entity deadEntity, int) {
		if (m_pRegistry->HasTag(deadEntity, QBERT_TAG))
		{
			switch (m_GameMode)
			{
			case GameMode::Normal:
				m_pRegistry->GetSystem<EnemySpawnerSystem>()->Reset();
				break;
			case GameMode::Versus:
				HandleEndRound(false);
			break;
			default:
				break;
			}
		}
		else
		{
			if (m_GameMode == GameMode::Versus)
			{
				HandleEndRound(true);
			}
		}
		});

	pLivesSystem->OnPlayerDied.Subscribe([this](Entity entity) {
		if (m_GameMode == GameMode::Coop)
		{
			m_NbrPlayerDead++;
			if (m_NbrPlayerDead < 2)
			{
				return;
			}
		}

		Timer::Get().SetTimeScale(0);
		if (m_GameMode == GameMode::Versus)
		{
			if (m_pRegistry->HasTag(entity, QBERT_TAG))
			{
				HandleEndRound(false);
			}
			else
			{
				HandleEndRound(true);
			}
		}
		else
		{
			OnGameOver.Notify();
		}
		});
}

void GameManagerSystem::HandleEndRound(bool bPlayer1Wins)
{
	Entity coilyEntity = m_pRegistry->FindComponentOfType<CoilyComponent>()->GetEntity();

	if (bPlayer1Wins)
	{
		m_pQbert->AddPoints(1);
	}
	else
	{
		m_pRegistry->GetComponent<CharacterPoint>(coilyEntity)->AddPoints(1);
	}
	
	ResetGame();
	ResetVersusCoily(coilyEntity);
	
}

void GameManagerSystem::ResetVersusCoily(Entity entity)
{
	m_pRegistry->GetComponent<MovementComponent>(entity)->CurrentQube = m_pPyramid->GetQubeAtIndex(2);
	m_pRegistry->GetComponent<CoilyComponent>(entity)->SetActive(false);
	m_pRegistry->GetComponent<AiControllerComponent>(entity)->SetActive(true);
	m_pRegistry->GetComponent<CharacterControllerComponent>(entity)->SetActive(false);

	m_pRegistry->GetSystem<CoilySystem>()->ResetCoily(entity);
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
		OnGameEnded.Notify("You win");
		Timer::Get().SetTimeScale(0.f);
		break;
	default:
		break;
	}
}