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
	m_pRegistry->GetSystem<QBertSystem>()->OnQBertEncounteredEnemy.Subscribe([this](Entity qbertEntity) {
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

	auto view = m_pRegistry->GetView<AiControllerComponent, MovementComponent>();

	for (Entity entity : view)
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
		pController->MoveTimer += TimerLocator::Get()->GetDeltaTime();
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
