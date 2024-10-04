#include "PCH.h"

#include "QBertSystem.h"
#include "PyramidSystem.h"
#include "CharacterMovementSystem.h"
#include "JumperSystem.h"
#include "DiskSystem.h"
#include "LivesSystem.h"

#include "AudioComponent.h"
#include "CharacterControllerComponent.h"
#include "QbertComponent.h"
#include "QubeComponent.h"
#include "MovementComponent.h"
#include "RendererComponent.h"
#include "CharacterLives.h"
#include "CharacterPoint.h"
#include "AiControllerComponent.h"
#include "CoilyComponent.h"
#include "CharacterPoint.h"
#include "ColliderComponent.h"

#include "Registry.h"

void QBertSystem::Start()
{
	Entity entity = *m_Entities.begin();

	auto* const pMoveComponent = m_pRegistry->GetComponent<MovementComponent>(entity);
	pMoveComponent->CurrentQube = m_pRegistry->GetSystem<PyramidSystem>()->GetTop();

	auto pQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComponent->CurrentQube);
	pQube->Characters.insert(entity);

	auto pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
	pTransform->Translate(pQube->CharacterPos);

	auto* const pMoveSystem = m_pRegistry->GetSystem<CharacterMovementSystem>();

	pMoveSystem->OnMoveStarted.Subscribe([this](Entity entity) {
		if (!m_pRegistry->HasTag(entity, QBERT_TAG))
		{
			return;
		}
		OnJumped(entity);
	});

	pMoveSystem->OnMeetCharacter.Subscribe([this](Entity characterEntity, std::unordered_set<Entity> const& qubeEntity) {
		HandleEnemyEncounter(characterEntity, qubeEntity);
	});

	auto* const pJumper = m_pRegistry->GetSystem<JumperSystem>();

	pJumper->OnJumpedToDeath.Subscribe([this](Entity entity) {
		if (!m_pRegistry->HasTag(entity, QBERT_TAG))
		{
			return;
		}
		OnJumpedToDeath(entity);
	});

	pJumper->OnFell.Subscribe([this](Entity entity) {
		OnFell(entity);
	});

	m_pRegistry->GetSystem<DiskSystem>()->OnDiskReachedTop.Subscribe([this, entity](Entity diskEntity) {
		JumpOffDisk(entity);
	});

	m_pRegistry->GetSystem<LivesSystem>()->OnDied.Subscribe([this](Entity entity, int lives) {
		if (!m_pRegistry->HasTag(entity, QBERT_TAG))
		{
			return;
		}

		Reset(false, m_pRegistry->GetComponent<MovementComponent>(entity)->CurrentQube);
		auto* const pQbert = m_pRegistry->GetComponent<QbertComponent>(entity);
		pQbert->bOnResetCoolDown = false;
		pQbert->ResetTimer = 0.f;
	});

	m_pRegistry->GetComponent<ColliderComponent>(entity)->OnTriggerEnter.Subscribe([this, entity](Entity otherEntity) {
		if (m_pRegistry->HasTag(otherEntity, ENEMY_TAG))
		{
			GetHurt(entity);
		}
	});
}

void QBertSystem::Update()
{
	Entity entity = *m_Entities.begin();
	auto* const pQbert = m_pRegistry->GetComponent<QbertComponent>(entity);

	if (pQbert->bOnResetCoolDown)
	{
		if (pQbert->ResetTimer >= pQbert->MAX_RESET_TIME)
		{
			m_pRegistry->GetComponent<CharacterLives>(entity)->Die();
		}
		else 
		{
			pQbert->ResetTimer += Timer::Get().GetDeltaTime();
		}
	}
}

void QBertSystem::Reset(bool fullReset, Entity targetQubeEntity)
{
	Entity entity = *m_Entities.begin();

	auto* const pMovement = m_pRegistry->GetComponent<MovementComponent>(entity);

	pMovement->CurrentQube = targetQubeEntity;
	pMovement->bCanMove = true;
	pMovement->CurrentDirection = ConnectionDirection::null;

	m_pRegistry->SetEntityActive(entity, true);
	m_pRegistry->GetComponent<RendererComponent>(entity)->Layer = 8;
	m_pRegistry->GetComponentInChildren<RendererComponent>(entity)->SetActive(false);

	if (!fullReset)
	{
		return;
	}

	auto* const pLives = m_pRegistry->GetComponent<CharacterLives>(entity);
	auto* const pPoints = m_pRegistry->GetComponent<CharacterPoint>(entity);
	pLives->Reset();
	pPoints->Reset();
}

void QBertSystem::OnJumped(Entity qbertEntity)
{
	auto* const pQbert = m_pRegistry->GetComponent<QbertComponent>(qbertEntity);
	pQbert->pJumpSound->Play();
}

void QBertSystem::HandleEnemyEncounter(Entity characterEntity, std::unordered_set<Entity> const& qubeCharacterEntities)
{
	Entity qbertEntity = *m_Entities.begin();

	auto* const pMover = m_pRegistry->GetComponent<MovementComponent>(qbertEntity);

	if (!pMover->bCanMove)
	{
		return;
	}

	std::vector<Entity> enemies{};

	if (characterEntity == qbertEntity)
	{
		std::copy(qubeCharacterEntities.begin(), qubeCharacterEntities.end(), std::back_inserter(enemies));
	}
	else if (qubeCharacterEntities.find(qbertEntity) != qubeCharacterEntities.end())
	{
		enemies.push_back(characterEntity);
		for (Entity entity : qubeCharacterEntities)
		{
			if (entity != qbertEntity)
			{
				enemies.push_back(entity);
			}
		}
	}
	else
	{
		return;
	}

	for (Entity enemyEntity : enemies)
	{
		auto* const pController = m_pRegistry->GetComponent<AiControllerComponent>(enemyEntity);

		if (!IS_VALID(pController))
		{
			return;
		}

		switch (pController->Type)
		{
		case EnemyType::Coily:
			if (m_pRegistry->GetComponent<CoilyComponent>(enemyEntity)->IsActive())
			{
				m_pRegistry->GetComponentInChildren<RendererComponent>(qbertEntity)->SetActive(true);
				m_pRegistry->GetComponent<MovementComponent>(qbertEntity)->bCanMove = false;

				auto* const pQBert = m_pRegistry->GetComponent<QbertComponent>(qbertEntity);
				pQBert->pSwearSound->Play();
				pQBert->bOnResetCoolDown = true;

				OnQBertEncounteredEnemy.Notify();
			}
			break;
		case EnemyType::SlickSam:
			m_pRegistry->GetComponent<CharacterLives>(enemyEntity)->Die();
			m_pRegistry->GetComponent<CharacterPoint>(qbertEntity)->AddPoints(pController->PointsForKill);
			break;
		default:
			break;
		}
	}
}

void QBertSystem::GetHurt(Entity qbertEntity)
{
	m_pRegistry->GetComponentInChildren<RendererComponent>(qbertEntity)->SetActive(true);
	m_pRegistry->GetComponent<MovementComponent>(qbertEntity)->bCanMove = false;

	auto* const pQBert = m_pRegistry->GetComponent<QbertComponent>(qbertEntity);
	pQBert->pSwearSound->Play();
	pQBert->bOnResetCoolDown = true;

	OnQBertEncounteredEnemy.Notify();
}

void QBertSystem::OnJumpedToDeath(Entity qbertEntity)
{
	auto* const pQbert = m_pRegistry->GetComponent<QbertComponent>(qbertEntity);
	pQbert->pFallSound->Play();

	m_pRegistry->GetComponent<RendererComponent>(qbertEntity)->Layer = 1;
}

void QBertSystem::OnFell(Entity qbertEntity)
{
	auto* const pLives = m_pRegistry->GetComponent<CharacterLives>(qbertEntity);
	if (IS_VALID(pLives))
	{
		pLives->Die();
	}
}

void QBertSystem::JumpOffDisk(Entity qbertEntity)
{
	auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(qbertEntity);
	auto* const pMove = m_pRegistry->GetComponent<MovementComponent>(qbertEntity);

	pTransform->SetParent(nullptr);
	pMove->CurrentQube = m_pRegistry->GetSystem<PyramidSystem>()->GetTop();
	pMove->bCanMove = true;
	pMove->CurrentDirection = ConnectionDirection::null;

	pTransform->Translate(m_pRegistry->GetComponent<QubeComponent>(pMove->CurrentQube)->CharacterPos);
}

void QBertSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<CharacterControllerComponent>());
	signature.set(m_pRegistry->GetComponentType<QbertComponent>());
	signature.set(m_pRegistry->GetComponentType<MovementComponent>());

	m_pRegistry->SetSystemSignature<QBertSystem>(signature);
}

void QBertSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(QBertSystem)).hash_code()));
}