#include "PCH.h"

#include "QBertSystem.h"
#include "PyramidSystem.h"
#include "CharacterMovementSystem.h"
#include "JumperSystem.h"
#include "DiskSystem.h"
#include "LivesSystem.h"
#include "GameManagerSystem.h"
#include "QubeSystem.h"

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
#include "DiskComponent.h"

#include "Registry.h"

void QBertSystem::Start()
{
	for (Entity entity : m_Entities)
	{
		m_pRegistry->GetSystem<DiskSystem>()->OnDiskReachedTop.Subscribe([this, entity](Entity diskEntity) {
			auto* const pDisk = m_pRegistry->GetComponent<DiskComponent>(diskEntity);
			JumpOffDisk(pDisk->QbertEntity);
		});

		m_pRegistry->GetComponent<ColliderComponent>(entity)->OnTriggerEnter.Subscribe([this, entity](Entity otherEntity) {
			if (m_pRegistry->HasTag(otherEntity, ENEMY_TAG))
			{	
				GetHurt(entity);
			}
		});
	}

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

	m_pRegistry->GetSystem<LivesSystem>()->OnDied.Subscribe([this](Entity entity, int lives) {
		if (!m_pRegistry->HasTag(entity, QBERT_TAG))
		{
			return;
		}

		auto* const pLives = m_pRegistry->GetComponent<CharacterLives>(entity);

		if (pLives->NbrLives > 0)
		{
			ResetToCurrentQube(entity);
		}

		auto* const pQbert = m_pRegistry->GetComponent<QbertComponent>(entity);
		pQbert->bOnResetCoolDown = false;
		pQbert->ResetTimer = 0.f;
	});

	m_pRegistry->GetSystem<PyramidSystem>()->OnAllQubesFlipped.Subscribe([this](int points) {
		for (Entity entity : m_Entities)
		{
			m_pRegistry->GetComponent<CharacterPoint>(entity)->AddPoints(points);
		}
	});
}

void QBertSystem::Update()
{
	for (Entity entity : m_Entities)
	{
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
}
void QBertSystem::ResetToCurrentQube(Entity qbertEntity)
{
	auto* const pMovement = m_pRegistry->GetComponent<MovementComponent>(qbertEntity);

	pMovement->bCanMove = true;
	pMovement->CurrentDirection = ConnectionDirection::null;

	m_pRegistry->GetComponent<QubeComponent>(pMovement->CurrentQube)->Characters.insert(qbertEntity);

	m_pRegistry->SetEntityActive(qbertEntity, true);
	m_pRegistry->GetComponent<RendererComponent>(qbertEntity)->Layer = 8;
	m_pRegistry->GetComponentInChildren<RendererComponent>(qbertEntity)->SetActive(false);
}

void QBertSystem::Reset(GameMode gameMode)
{
	auto* const pPyramid = m_pRegistry->GetSystem<PyramidSystem>();
	switch (gameMode)
	{
	case GameMode::Normal:
		for (Entity entity : m_Entities)
		{
			SetToQube(entity, pPyramid->GetTop());
		}
		break;
	case GameMode::Coop:
		SetStartQubes(gameMode);
		for (Entity entity : m_Entities)
		{
			ResetToCurrentQube(entity);
		}
		break;
	case GameMode::Versus:
		for (Entity entity : m_Entities)
		{
			SetToQube(entity, pPyramid->GetQubeAtIndex(1));
		}
		break;
	default:
		break;
	}
}

void QBertSystem::SetToQube(Entity qbertEntity, Entity qubeEntity)
{
	auto* const pMovement = m_pRegistry->GetComponent<MovementComponent>(qbertEntity);

	pMovement->CurrentQube = qubeEntity;
	pMovement->bCanMove = true;
	pMovement->CurrentDirection = ConnectionDirection::null;

	m_pRegistry->SetEntityActive(qbertEntity, true);
	m_pRegistry->GetComponent<RendererComponent>(qbertEntity)->Layer = 8;
	m_pRegistry->GetComponentInChildren<RendererComponent>(qbertEntity)->SetActive(false);
}

void QBertSystem::SetStartQubes(GameMode mode)
{
	auto* const pPyramid = m_pRegistry->GetSystem<PyramidSystem>();
	switch (mode)
	{
	case GameMode::Normal:
		for (Entity entity : m_Entities) //should only be one
		{
			m_pRegistry->GetComponent<MovementComponent>(entity)->CurrentQube = pPyramid->GetTop();
		}
		break;
	case GameMode::Coop:
	{
		int count = 0; // we assume we only have two players
		for (Entity entity : m_Entities)
		{
			if (count == 0)
			{
				m_pRegistry->GetComponent<MovementComponent>(entity)->CurrentQube = pPyramid->GetSpawnQube(MovementMode::EscheresqueLeft);
			}
			else
			{
				m_pRegistry->GetComponent<MovementComponent>(entity)->CurrentQube = pPyramid->GetSpawnQube(MovementMode::EscheresqueRight);
			}

			count++;
		}
		break;
	}
	case GameMode::Versus:
		for (Entity entity : m_Entities) // should only be one
		{
			m_pRegistry->GetComponent<MovementComponent>(entity)->CurrentQube = pPyramid->GetQubeAtIndex(1);
		}
		break;
	default:
		break;
	}

	for (Entity entity : m_Entities)
	{
		auto pQube = m_pRegistry->GetComponent<QubeComponent>(m_pRegistry->GetComponent<MovementComponent>(entity)->CurrentQube);
		pQube->Characters.insert(entity);

		auto pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
		pTransform->Translate(pQube->CharacterPos);
	}
}

void QBertSystem::AddPoints(int points)
{
	for (Entity entity : m_Entities)
	{
		m_pRegistry->GetComponent<CharacterPoint>(entity)->AddPoints(points);
	}
}

void QBertSystem::OnJumped(Entity qbertEntity)
{
	auto* const pQbert = m_pRegistry->GetComponent<QbertComponent>(qbertEntity);
	pQbert->pJumpSound->Play();
}

void QBertSystem::HandleEnemyEncounter(Entity characterEntity, std::unordered_set<Entity> const& qubeCharacterEntities)
{
	for (Entity qbertEntity : m_Entities)
	{
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
			continue;
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
					GetHurt(qbertEntity);
				}
				else
				{
					auto* const pCharacterController = m_pRegistry->GetComponent<CharacterControllerComponent>(enemyEntity);
					if (IS_VALID(pCharacterController) && pCharacterController->IsActive())
					{
						GetHurt(qbertEntity);
					}
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


	auto* const pMovement = m_pRegistry->GetComponent<MovementComponent>(qbertEntity);
	auto* const pCurrentQube = m_pRegistry->GetComponent<QubeComponent>(pMovement->CurrentQube);

	if (!pCurrentQube->bIsLastRow)
	{
		m_pRegistry->GetComponent<RendererComponent>(qbertEntity)->Layer = 1;
	}
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