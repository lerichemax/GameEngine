#include "PCH.h"
#include "EnemySpawnerSystem.h"
#include "PyramidSystem.h"
#include "JumperSystem.h"
#include "CharacterMovementSystem.h"
#include "LivesSystem.h"
#include "DiskSystem.h"

#include "EnemySpawnerComponent.h"
#include "MovementComponent.h"
#include "QubeComponent.h"
#include "CharacterLives.h"
#include "AiControllerComponent.h"
#include "QbertComponent.h"
#include "JumpComponent.h"

#include "Timer.h"

void EnemySpawnerSystem::Start()
{
	m_pPyramid = m_pRegistry->GetSystem<PyramidSystem>();

	m_pPyramid->OnAllQubesFlipped.Subscribe([this](int) {
		Reset();
		});

	for (Entity entity : m_Entities)
	{
		auto* const pSpawner = m_pRegistry->GetComponent<EnemySpawnerComponent>(entity);

		for (Entity enemyEntity : pSpawner->SpawnedEnemies)
		{
			m_pRegistry->SetEntityActive(enemyEntity, false);
		}
	}

	m_pRegistry->GetSystem<JumperSystem>()->OnFell.Subscribe([this](Entity entity) {
		if (!m_pRegistry->HasTag(entity, ENEMY_TAG)) 
		{
			return;
		}

		m_pRegistry->GetComponent<CharacterLives>(entity)->Die();
	});

	m_pRegistry->GetSystem<CharacterMovementSystem>()->OnJumpedOnDisk.Subscribe([this](Entity diskEntity) {
		bIsPaused = true;
	});

	m_pRegistry->GetSystem<DiskSystem>()->OnDiskReachedTop.Subscribe([this](Entity diskEntity) {
		bIsPaused = false;
	});

	m_pRegistry->GetSystem<LivesSystem>()->OnDied.Subscribe([this](Entity deadEntity, int nbrLives) {
		if (!m_pRegistry->HasTag(deadEntity, QBERT_TAG))
		{
			auto* const pAiController = m_pRegistry->GetComponent<AiControllerComponent>(deadEntity);
			auto entityIt = std::find_if(m_Entities.begin(), m_Entities.end(), [this, pAiController](Entity managedEntity) {
				return m_pRegistry->GetComponent<EnemySpawnerComponent>(managedEntity)->Type == pAiController->Type;
				});

			m_pRegistry->GetComponent<JumpComponent>(deadEntity)->Reset();


			m_pRegistry->GetComponent<EnemySpawnerComponent>(*entityIt)->NbrEnemies--;
			m_pRegistry->SetEntityActive(deadEntity, false);
		}
	});
}

void EnemySpawnerSystem::Update()
{
	if (bIsPaused)
	{
		return;
	}

	for (Entity entity : m_Entities)
	{
		auto* const pSpawnerComp = m_pRegistry->GetComponent<EnemySpawnerComponent>(entity);

		if (pSpawnerComp->NbrEnemies < pSpawnerComp->MaxEnemies)
		{
			if (pSpawnerComp->EnemySpawnTimer < pSpawnerComp->SpawnInterval)
			{
				pSpawnerComp->EnemySpawnTimer += Timer::Get().GetDeltaTime();
			}
			else
			{
				Spawn(pSpawnerComp);
				pSpawnerComp->EnemySpawnTimer = 0;
			}
		}
	}
}

void EnemySpawnerSystem::Spawn(EnemySpawnerComponent* const pSpawnerComp) const
{
	Entity enemyEntity = pSpawnerComp->SpawnedEnemies[pSpawnerComp->NbrEnemies];

	m_pRegistry->GetComponent<CharacterLives>(enemyEntity)->Reset();
	
	auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(enemyEntity);

	Entity qubeEntity = m_pPyramid->GetSpawnQube(pMoveComp->Mode);

	pMoveComp->CurrentQube = qubeEntity;
	pMoveComp->bCanMove = true;

	auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(enemyEntity);
	auto* const pCurrentQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);
	pCurrentQube->Characters.insert(enemyEntity);

	pTransform->Translate(pCurrentQube->GetEnemyTopPosition(pMoveComp->Mode));

	m_pRegistry->SetEntityActive(enemyEntity, true);
	OnEnemySpawned.Notify(enemyEntity);

	pSpawnerComp->NbrEnemies++;
}

void EnemySpawnerSystem::Reset()
{
	for (Entity entity : m_Entities)
	{
		auto* const pSpawnerComp = m_pRegistry->GetComponent<EnemySpawnerComponent>(entity);

		for (Entity enemyEntity : pSpawnerComp->SpawnedEnemies)
		{
			m_pRegistry->SetEntityActive(enemyEntity, false);
			pSpawnerComp->EnemySpawnTimer = 0;
			pSpawnerComp->NbrEnemies = 0;
		}
	}
}

void EnemySpawnerSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<EnemySpawnerComponent>());

	m_pRegistry->SetSystemSignature<EnemySpawnerSystem>(signature);
}

void EnemySpawnerSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(EnemySpawnerSystem)).hash_code()));
}