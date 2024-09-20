#include "PCH.h"
#include "CoilySystem.h"
#include "JumperSystem.h"
#include "PyramidSystem.h"
#include "EnemySpawnerSystem.h"
#include "CharacterLives.h"
#include "LivesSystem.h"
#include "DiskSystem.h"
#include "CharacterMovementSystem.h"

#include "AiControllerComponent.h"
#include "JumpComponent.h"
#include "MovementComponent.h"
#include "CoilyComponent.h"
#include "QbertComponent.h"
#include "AudioComponent.h"
#include "QubeComponent.h"
#include "RendererComponent.h"
#include "DiskComponent.h"

#include <thread>

void CoilySystem::Start()
{
	m_pPyramid = m_pRegistry->GetSystem<PyramidSystem>();
	m_Qbert = FindComponentOfType<QbertComponent>()->GetGameObject()->GetEntity();

	auto* const pJumper = m_pRegistry->GetSystem<JumperSystem>();
	pJumper->OnJumpedToDeath.Subscribe([this](Entity entity) {
		if (!m_pRegistry->HasTag(entity, ENEMY_TAG))
		{
			return;
		}
		HandleJumpToDeath(entity);
	});

	pJumper->OnJumpLanded.Subscribe([this](Entity entity) {
		if (!m_pRegistry->HasTag(entity, ENEMY_TAG))
		{
			return;
		}
		auto* const pEnemy = m_pRegistry->GetComponent<CoilyComponent>(entity);
		if (IS_VALID(pEnemy) && pEnemy->IsActive())
		{
			SearchForQbert(entity);
		}
	});

	m_pRegistry->GetSystem<EnemySpawnerSystem>()->OnEnemySpawned.Subscribe([this](Entity entity) {
		auto* const pEnemy = m_pRegistry->GetComponent<CoilyComponent>(entity);
		if (IS_VALID(pEnemy))
		{
			pEnemy->SetActive(false);
		}
	});

	m_pRegistry->GetSystem<LivesSystem>()->OnDied.Subscribe([this](Entity entity, int nbrLives) {
		auto* const pCoily = m_pRegistry->GetComponent<CoilyComponent>(entity);
		if (IS_VALID(pCoily) || m_pRegistry->HasTag(entity, QBERT_TAG))
		{
			ResetCoily(*m_Entities.begin());
		}
	});

	m_pRegistry->GetSystem<DiskSystem>()->OnDiskReachedTop.Subscribe([this](Entity entity) {
		SearchForQbert(*m_Entities.begin());
		});
}

void CoilySystem::Update()
{
	if (m_Entities.empty())
	{
		return;
	}

	Entity coilyEntity = *m_Entities.begin();

	auto* const pAiControllerComp = m_pRegistry->GetComponent<AiControllerComponent>(coilyEntity);
	auto* const pCoily = m_pRegistry->GetComponent<CoilyComponent>(coilyEntity);

	if (pCoily->IsActive() || (pAiControllerComp->IsActive() && !pCoily->IsActive()))
	{
		auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(coilyEntity);

		HandleAi(pMoveComp, pAiControllerComp);
	}
}

void CoilySystem::HandleJumpToDeath(Entity coilyEntity)
{
	auto* const pCoily = m_pRegistry->GetComponent<CoilyComponent>(coilyEntity);

	if (!IS_VALID(pCoily))
	{
		return;
	}

	if (!pCoily->IsActive())
	{
		HandleCoilyTransform(coilyEntity);
		SearchForQbert(coilyEntity);
	}
	else
	{
		m_pRegistry->GetComponent<AudioComponent>(coilyEntity)->Play();
		m_pRegistry->GetComponent<RendererComponent>(coilyEntity)->Layer = 1;
	}
}

void CoilySystem::HandleCoilyTransform(Entity entity)
{
	m_pRegistry->GetComponent<JumpComponent>(entity)->Reset();
	
	auto* const pMovement = m_pRegistry->GetComponent<MovementComponent>(entity);

	pMovement->SetTextureIdleNames("Textures/Enemies/Coily/Coily_Small_DownRight.png", 
		"Textures/Enemies/Coily/Coily_Small_DownLeft.png",
		"Textures/Enemies/Coily/Coily_Small_UpRight.png",
		"Textures/Enemies/Coily/Coily_Small_UpLeft.png");

	pMovement->SetTextureJumpNames("Textures/Enemies/Coily/Coily_Big_DownRight.png", 
		"Textures/Enemies/Coily/Coily_Big_DownLeft.png", 
		"Textures/Enemies/Coily/Coily_Big_UpRight.png", 
		"Textures/Enemies/Coily/Coily_Big_DownLeft.png");

	pMovement->bCanMove = true;
	pMovement->CurrentDirection = ConnectionDirection::null;

	m_pRegistry->GetComponent<CoilyComponent>(entity)->SetActive(true);
	m_pRegistry->GetComponent<AiControllerComponent>(entity)->SetActive(false);
}

void CoilySystem::ResetCoily(Entity entity)
{
	auto* const pMovement = m_pRegistry->GetComponent<MovementComponent>(entity);

	pMovement->SetTextureIdleNames("Textures/Enemies/Coily/Coily_Egg_Small.png", "Textures/Enemies/Coily/Coily_Egg_Small.png", "", "");
	pMovement->SetTextureJumpNames("Textures/Enemies/Coily/Coily_Egg_Big.png", "Textures/Enemies/Coily/Coily_Egg_Big.png", "", "");

	pMovement->bCanMove = true;

	auto* const pCoily = m_pRegistry->GetComponent<CoilyComponent>(entity);
	pCoily->CurrentlyInQueue = 0;
	pCoily->SetActive(false);

	m_pRegistry->GetComponent<RendererComponent>(entity)->Layer = 7;
}
void CoilySystem::SearchForQbert(Entity entity)
{
	auto* const pCoily = m_pRegistry->GetComponent<CoilyComponent>(entity);

	if (pCoily->CurrentlyInQueue != 0)
	{
		return;
	}
	
	auto* const pMover = m_pRegistry->GetComponent<MovementComponent>(entity);
	std::thread t1([this, entity, pCoily, pMover]
		{
			Entity targetEntity = NULL_ENTITY;

			auto* const pQbertMoveComp = m_pRegistry->GetComponent<MovementComponent>(m_Qbert);

			if (pQbertMoveComp->CurrentQube != NULL_ENTITY)
			{
				targetEntity = pQbertMoveComp->CurrentQube;
			}
			else
			{
				auto* const pQbert = m_pRegistry->GetComponent<QbertComponent>(m_Qbert);

				targetEntity = pQbert->Disk;
			}

			if (pMover->CurrentQube == pQbertMoveComp->CurrentQube)
			{
				return;
			}

			if (m_pPyramid->TryFindPathTo(pMover->CurrentQube, targetEntity, pCoily->MovementQueue, pCoily->MOVEMENT_QUEUE_SIZE))
			{
				pCoily->CurrentlyInQueue = pCoily->MOVEMENT_QUEUE_SIZE;
			}
		});
	t1.join();
}

void CoilySystem::ChooseDirection(MovementComponent* const pMover) const
{
	if (!m_pRegistry->GetComponent<CoilyComponent>(*m_Entities.begin())->IsActive())
	{
		AiControllerSystem::ChooseDirection(pMover);
		return;
	}

	Entity coilyEntity = *m_Entities.begin();

	auto* const pCoily = m_pRegistry->GetComponent<CoilyComponent>(coilyEntity);
	
	if (pCoily->MOVEMENT_QUEUE_SIZE - pCoily->CurrentlyInQueue == pCoily->MOVEMENT_QUEUE_SIZE)
	{
		pMover->CurrentDirection = ConnectionDirection::null;
		return;
	}

	pMover->CurrentDirection = pCoily->MovementQueue[pCoily->MOVEMENT_QUEUE_SIZE - pCoily->CurrentlyInQueue];
	pCoily->CurrentlyInQueue--;
}

void CoilySystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(CoilySystem)).hash_code()));
}

void CoilySystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<AiControllerComponent>());
	signature.set(m_pRegistry->GetComponentType<MovementComponent>());
	signature.set(m_pRegistry->GetComponentType<CoilyComponent>());
	m_pRegistry->SetSystemSignature<CoilySystem>(signature);
}