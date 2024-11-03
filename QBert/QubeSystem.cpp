#include "PCH.h"
#include "QubeSystem.h"
#include "JumperSystem.h"
#include "LivesSystem.h"
#include "GameManagerSystem.h"

#include "QubeComponent.h"
#include "MovementComponent.h"
#include "QbertComponent.h"
#include "AiControllerComponent.h"
#include "RendererComponent.h"

#include "Texture2D.h"


#include "GameObject.h"
#include "PrefabsManager.h"
#include "ResourceManager.h"


#include "Registry.h"

EventHandler<QubeSystem> QubeSystem::OnAnyQubeFlipped{};

void QubeSystem::Initialize()
{
	for (Entity entity : m_Entities)
	{
		auto* const pTextureRenderer = m_pRegistry->GetComponent<RendererComponent>(entity);
		auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(entity);
		auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

		pTextureRenderer->pTexture = pQube->pDefaultText;

		pQube->CharacterPos.x = pTransform->GetLocation().x + pTextureRenderer->pTexture->GetWidth() / 4;
		pQube->CharacterPos.y = pTransform->GetLocation().y - pTextureRenderer->pTexture->GetHeight() / 5;

		pQube->EscheresqueRightPos.x = pTransform->GetLocation().x + (pTextureRenderer->pTexture->GetWidth() * 1.4f);
		pQube->EscheresqueRightPos.y = pTransform->GetLocation().y + pTextureRenderer->pTexture->GetHeight();
		
		pQube->EscheresqueLeftPos.x = pTransform->GetLocation().x + (pTextureRenderer->pTexture->GetWidth() / 5.f);
		pQube->EscheresqueLeftPos.y = pTransform->GetLocation().y + pTextureRenderer->pTexture->GetHeight();
	}
}

void QubeSystem::Start()
{
	m_pRegistry->GetSystem<JumperSystem>()->OnJumpLanded.Subscribe([this](Entity entity) {

		auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);
		auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);

		pQube->Characters.insert(entity);

		if (m_pRegistry->EntityHasTag(entity, QBERT_TAG))
		{
			HandleQBertLanding(pMoveComp->CurrentQube);
		}

		if (m_pRegistry->EntityHasTag(entity, ENEMY_TAG))
		{
			auto* const pEnemy = m_pRegistry->GetComponent<AiControllerComponent>(entity);
			if (pEnemy->Type == EnemyType::SlickSam && pQube->bIsFlipped)
			{
				UnFlip(pMoveComp->CurrentQube);
			}
		}
	});

	m_pRegistry->GetSystem<LivesSystem>()->OnDied.Subscribe([this](Entity deadEntity, int nbrLives) {
		for (Entity entity : m_Entities)
		{
			auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(entity);
			if (pQube->Characters.contains(deadEntity) && m_pRegistry->EntityHasTag(deadEntity, QBERT_TAG))
			{
				pQube->Characters.clear();
				pQube->Characters.insert(deadEntity);
			}
			else
			{
				pQube->Characters.clear();
			}
		}
	});
}

void QubeSystem::HandleQBertLanding(Entity qubeEntity)
{
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(qubeEntity);
	switch (pQube->QubeLevel)
	{
	case Level::Level1:
		Flip(qubeEntity);
		break;
	case Level::Level2:
		if (pQube->JumpCounter < 1)
		{
			auto* const pRenderer = m_pRegistry->GetComponent<RendererComponent>(qubeEntity);

			pRenderer->pTexture = pQube->pIntermediateTexture;
			pQube->JumpCounter++;
		}
		else 
		{
			Flip(qubeEntity);
		}
		break;
	case Level::Level3:
		if (pQube->bIsFlipped)
		{
			UnFlip(qubeEntity);
		}
		else {
			Flip(qubeEntity);
		}
		break;
	}
}


void QubeSystem::Flip(Entity qubeEntity)
{
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(qubeEntity);
	auto* const pRenderer = m_pRegistry->GetComponent<RendererComponent>(qubeEntity);

	if (!pQube->bIsFlipped)
	{
		pRenderer->pTexture = pQube->pFlippedTexture;
		pQube->bIsFlipped = true;
		pQube->JumpCounter++;
		OnAnyQubeFlipped.Notify();
	}
}

void QubeSystem::UnFlip(Entity qubeEntity)
{
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(qubeEntity);
	auto* const pRenderer = m_pRegistry->GetComponent<RendererComponent>(qubeEntity);

	pQube->bIsFlipped = false;
	pRenderer->pTexture = pQube->pDefaultText;
	pQube->JumpCounter=0;
}

void QubeSystem::Reset(Level level, Entity entity)
{
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(entity);

	pQube->bIsFlipped = false;
	pQube->JumpCounter = 0;
	pQube->QubeLevel = level;

	auto* const pRenderer = m_pRegistry->GetComponent<RendererComponent>(entity);
	
	pRenderer->pTexture = pQube->pDefaultText;
}

void QubeSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(QubeSystem)).hash_code()));
}

void QubeSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<QubeComponent>());
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());
	signature.set(m_pRegistry->GetComponentType<RendererComponent>());

	m_pRegistry->SetSystemSignature<QubeSystem>(signature);
}
