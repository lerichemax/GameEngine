#include "PCH.h"
#include "AiControllerSystem.h"
#include "QBertSystem.h"
#include "LivesSystem.h"

#include "AiControllerComponent.h"
#include "MovementComponent.h"
#include "QubeComponent.h"

#include "Timer.h"

void AiControllerSystem::Start()
{
	m_pRegistry->GetSystem<QBertSystem>()->OnQBertEncounteredEnemy.Subscribe([this]() {
		m_bIsPaused = true;
		});

	m_pRegistry->GetSystem<LivesSystem>()->OnDied.Subscribe([this](Entity entity, int lives) {
		m_bIsPaused = false;
		});
}

void AiControllerSystem::Update()
{
	if (m_bIsPaused)
	{
		return;
	}
	for (Entity entity : m_Entities)
	{
		auto* const pAiControllerComp = m_pRegistry->GetComponent<AiControllerComponent>(entity);

		if (pAiControllerComp->Type == EnemyType::Coily) // clumsy
		{
			continue;
		}

		auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);

		if (!pAiControllerComp->IsActive())
		{
			continue;
		}

		HandleAi(pMoveComp, pAiControllerComp);
	}
}

void AiControllerSystem::HandleAi(MovementComponent* const pMovement, AiControllerComponent* const pController)
{
	if (!pMovement->bCanMove)
	{
		return;
	}

	if (pController->MoveTimer < pController->MOVE_INTERVAL)
	{
		pController->MoveTimer += Timer::GetInstance().GetDeltaTime();
	}
	else
	{
		ChooseDirection(pMovement);
		pController->MoveTimer = 0;
	}
}

void AiControllerSystem::ChooseDirection(MovementComponent* const pMover) const
{
	int random = rand() % 2 + 1; //down left or down right

	auto dir = static_cast<ConnectionDirection>(random);

	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(pMover->CurrentQube);

	if (!IS_VALID(pQube))
	{
		pMover->CurrentDirection = ConnectionDirection::null;
		return;
	}
	
	if (!pQube->HasConnection(dir))
	{
		if (dir == ConnectionDirection::downLeft)
		{
			pMover->CurrentDirection = ConnectionDirection::downRight;
		}
		else if (dir == ConnectionDirection::downRight)
		{
			pMover->CurrentDirection = ConnectionDirection::downLeft;
		}
	}

	pMover->CurrentDirection = dir;
}

void AiControllerSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(AiControllerSystem)).hash_code()));
}

void AiControllerSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<AiControllerComponent>());
	signature.set(m_pRegistry->GetComponentType<MovementComponent>());
	m_pRegistry->SetSystemSignature<AiControllerSystem>(signature);
}
