#include "PCH.h"
#include "EnemySpawnerSystem.h"
#include "PyramidSystem.h"
#include "JumperSystem.h"
#include "AiControllerComponent.h"

#include "EnemySpawnerComponent.h"
#include "MovementComponent.h"
#include "QubeComponent.h"

#include "Timer.h"

void EnemySpawnerSystem::Start()
{
	m_pPyramid = m_pRegistry->GetSystem<PyramidSystem>();

	for (Entity entity : m_Entities)
	{
		auto* const pSpawner = m_pRegistry->GetComponent<EnemySpawnerComponent>(entity);

		for (Entity enemyEntity : pSpawner->SpawnedEnemies)
		{
			m_pRegistry->SetEntityActive(enemyEntity, false);
		}
	}

	m_pRegistry->GetSystem<JumperSystem>()->OnJumpedToDeath.Subscribe([this](Entity entity) {
		if (!m_pRegistry->HasTag(entity, ENEMY_TAG)) 
		{
			return;
		}

		auto* const pEnemy = m_pRegistry->GetComponent<AiControllerComponent>(entity);

		auto entityIt = std::find_if(m_Entities.begin(), m_Entities.end(), [this, pEnemy](Entity managedEntity) {
			return m_pRegistry->GetComponent<EnemySpawnerComponent>(managedEntity)->Type == pEnemy->Type;
			});

		m_pRegistry->GetComponent<EnemySpawnerComponent>(*entityIt)->NbrEnemies--;
	});
}

void EnemySpawnerSystem::Update()
{
	for (Entity entity : m_Entities)
	{
		auto* const pSpawnerComp = m_pRegistry->GetComponent<EnemySpawnerComponent>(entity);

		if (pSpawnerComp->NbrEnemies < pSpawnerComp->MaxEnemies)
		{
			if (pSpawnerComp->EnemySpawnTimer < pSpawnerComp->SpawnInterval)
			{
				pSpawnerComp->EnemySpawnTimer += Timer::GetInstance().GetDeltaTime();
			}
			else
			{
				Spawn(pSpawnerComp);
				pSpawnerComp->EnemySpawnTimer = 0;
				pSpawnerComp->NbrEnemies++;
			}
		}
	}
}

void EnemySpawnerSystem::Spawn(EnemySpawnerComponent* const pSpawnerComp) const
{
	Entity qubeEntity = m_pPyramid->GetRandomTopQube();

	if (qubeEntity == EntityManager::NULL_ENTITY)
	{
		return;
	}

	Entity enemyEntity = pSpawnerComp->SpawnedEnemies[pSpawnerComp->NbrEnemies];
	m_pRegistry->SetEntityActive(enemyEntity, true);

	auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(enemyEntity);
	
	pMoveComp->CurrentQube = qubeEntity;
	pMoveComp->bCanMove = true;

	auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(enemyEntity);
	auto* const pCurrentQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);
	pCurrentQube->bIsOccupied = true;

	pTransform->Translate(pCurrentQube->CharacterPos);

	m_pRegistry->SetEntityActive(enemyEntity, true);
}

//void EnemySpawnerSystem::EnemyDied(Enemy* pEnemy)
//{
//	//if (!m_pEnemies.empty())
//	//{
//	//	for (size_t i{}; i < MAX_ENEMY_OF_TYPE; i++)
//	//	{
//	//		if (m_pEnemies[i] == pEnemy)
//	//		{
//	//			m_pEnemies[i] = nullptr;
//	//			break;
//	//		}
//	//	}
//	//}
//	//m_NbrEnemies--;
//}

void EnemySpawnerSystem::Reset()
{
	//if (!m_pEnemies.empty())
	//{
	//	for (size_t i{}; i < MAX_ENEMY_OF_TYPE; i++)
	//	{
	//		if (m_pEnemies[i] != nullptr)
	//		{
	//			m_pEnemies[i]->Die();
	//		}
	//	}
	//}
	//m_NbrEnemies = 0;
}

void EnemySpawnerSystem::ResetTimer()
{
	//m_EnemySpawnTimer = 0;
}

void EnemySpawnerSystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<EnemySpawnerComponent>());

	m_pRegistry->SetSystemSignature<EnemySpawnerSystem>(signature);
}

void EnemySpawnerSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(EnemySpawnerSystem)).hash_code()));
}