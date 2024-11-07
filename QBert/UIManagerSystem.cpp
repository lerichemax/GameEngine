#include "PCH.h"
#include "UIManagerSystem.h"
#include "GameManagerSystem.h"
#include "LivesSystem.h"

#include "CharacterPoint.h"
#include "CharacterLives.h"
#include "TextRendererComponent.h"
#include "UiManagerComponent.h"
#include "CharacterControllerComponent.h"

void UIManagerSystem::Start()
{
	auto view = m_pRegistry->GetView< UiManagerComponent>();

	m_UiEntity = *view.begin();

	for (CharacterPoint* pPointComp : m_pRegistry->FindComponentsOfType<CharacterPoint>())
	{
		pPointComp->OnPointsUpdated.Subscribe([this](int points, Entity playerEntity) {
			auto* const pUiComp = m_pRegistry->GetComponent<UiManagerComponent>(m_UiEntity);

			auto* const pCharacter = m_pRegistry->GetComponent<CharacterControllerComponent>(playerEntity);

			if (pCharacter->PlayerNumber == 1)
			{
				auto* const pText = m_pRegistry->GetComponent<TextRendererComponent>(pUiComp->PointsCounterTextEntity);

				pText->SetText("P1 Points: " + std::to_string(points));
			}
			else if (pCharacter->PlayerNumber == 2 && pUiComp->PointsCounterTextEntityP2 != NULL_ENTITY)
			{
				auto* const pText = m_pRegistry->GetComponent<TextRendererComponent>(pUiComp->PointsCounterTextEntityP2);

				pText->SetText("P2 Points: " + std::to_string(points));
			}
		});
	}

	auto* const pLivesSystem = m_pRegistry->GetSystem<LivesSystem>();

	pLivesSystem->OnDied.Subscribe([this](Entity deadEntity, int lives) {
		auto* const pCharacter = m_pRegistry->GetComponent<CharacterControllerComponent>(deadEntity);

		if (!IS_VALID(pCharacter))
		{
			return;
		}

		auto* const pUiComp = m_pRegistry->GetComponent<UiManagerComponent>(m_UiEntity);

		if (pCharacter->PlayerNumber == 1 && pUiComp->LivesCounterTextEntity != NULL_ENTITY)
		{
			auto* const pText = m_pRegistry->GetComponent<TextRendererComponent>(pUiComp->LivesCounterTextEntity);

			pText->SetText("P1 Lives: " + std::to_string(lives));
		}
		else if (pCharacter->PlayerNumber == 2 && pUiComp->LivesCounterTextEntityP2 != NULL_ENTITY)
		{
			auto* const pText = m_pRegistry->GetComponent<TextRendererComponent>(pUiComp->LivesCounterTextEntityP2);

			pText->SetText("P2 Lives: " + std::to_string(lives));
		}
	});

	auto* const pGameManager = m_pRegistry->GetSystem<GameManagerSystem>();
	if (!IS_VALID(pGameManager))
	{
		return;
	}

	pGameManager->OnGamePaused.Subscribe([this](bool isPaused) {
		auto* const pUiComp = m_pRegistry->GetComponent<UiManagerComponent>(m_UiEntity);

		m_pRegistry->SetEntityHierarchyActive(pUiComp->PauseMenuEntity, isPaused);
		});

	pGameManager->OnGameEnded.Subscribe([this](std::string const& endText) {
		auto* const pUiComp = m_pRegistry->GetComponent<UiManagerComponent>(m_UiEntity);
		auto* const pText = m_pRegistry->GetComponentInChildren<TextRendererComponent>(pUiComp->GameOverMenuEntity);

		pText->SetText(endText);

		m_pRegistry->SetEntityHierarchyActive(pUiComp->GameOverMenuEntity, true);
		});

	pGameManager->OnGameOver.Subscribe([this]() {
		auto* const pUiComp = m_pRegistry->GetComponent<UiManagerComponent>(m_UiEntity);
		auto* const pText = m_pRegistry->GetComponentInChildren<TextRendererComponent>(pUiComp->GameOverMenuEntity);

		pText->SetText("Game Over");

		m_pRegistry->SetEntityHierarchyActive(pUiComp->GameOverMenuEntity, true);
		});
}
