#include "PCH.h"
#include "UIManagerSystem.h"
#include "GameManagerSystem.h"
#include "LivesSystem.h"

#include "CharacterPoint.h"
#include "CharacterLives.h"
#include "TextRendererComponent.h"
#include "UiManagerComponent.h"
#include "QbertComponent.h"

void UIManagerSystem::Start()
{
	Entity entity = *m_Entities.cbegin();

	FindComponentOfType<CharacterPoint>()->OnPointsUpdated.Subscribe([this, entity](int points) {
		auto* const pUiComp = m_pRegistry->GetComponent<UiManagerComponent>(entity);
		auto* const pText = m_pRegistry->GetComponent<TextRendererComponent>(pUiComp->PointsCounterTextEntity);

		pText->SetText("P1 Points: " + std::to_string(points));
		});

	auto* const pLivesSystem = m_pRegistry->GetSystem<LivesSystem>();

	pLivesSystem->OnDied.Subscribe([this, entity](Entity deadEntity, int lives) {
		if (m_pRegistry->HasTag(deadEntity, QBERT_TAG))
		{
			auto* const pUiComp = m_pRegistry->GetComponent<UiManagerComponent>(entity);
			auto* const pText = m_pRegistry->GetComponent<TextRendererComponent>(pUiComp->LivesCounterTextEntity);

			pText->SetText("P1 Lives: " + std::to_string(lives));
		}
	});

	pLivesSystem->OnGameOver.Subscribe([this, entity]() {
		auto* const pUiComp = m_pRegistry->GetComponent<UiManagerComponent>(entity);
		auto* const pText = m_pRegistry->GetComponentInChildren<TextRendererComponent>(pUiComp->GameOverMenuEntity);

		pText->SetText("Game Over");

		m_pRegistry->SetEntityHierarchyActive(pUiComp->GameOverMenuEntity, true);
	});

	auto* const pGameManager = m_pRegistry->GetSystem<GameManagerSystem>();

	pGameManager->OnGamePaused.Subscribe([this, entity](bool isPaused) {
		auto* const pUiComp = m_pRegistry->GetComponent<UiManagerComponent>(entity);

		m_pRegistry->SetEntityHierarchyActive(pUiComp->PauseMenuEntity, isPaused);
		});

	pGameManager->OnGameEnded.Subscribe([this, entity]() {
		auto* const pUiComp = m_pRegistry->GetComponent<UiManagerComponent>(entity);
		auto* const pText = m_pRegistry->GetComponentInChildren<TextRendererComponent>(pUiComp->GameOverMenuEntity);

		pText->SetText("You Win");

		m_pRegistry->SetEntityHierarchyActive(pUiComp->GameOverMenuEntity, true);
	});
}

void UIManagerSystem::SetSignature() const
{
	Signature signature{};
	signature.set(m_pRegistry->GetComponentType<UiManagerComponent>());

	m_pRegistry->SetSystemSignature<UIManagerSystem>(signature);
}

void UIManagerSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(UIManagerSystem)).hash_code()));

	System::Serialize(writer);
}