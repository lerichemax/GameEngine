#include "PCH.h"
#include "AiControllerSystem.h"

#include "AiControllerComponent.h"
#include "MovementComponent.h"
#include "QubeComponent.h"

#include "Timer.h"

void AiControllerSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(AiControllerSystem)).hash_code()));
}

void AiControllerSystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<AiControllerComponent>());
	signature.set(m_pRegistry->GetComponentType<MovementComponent>());
	m_pRegistry->SetSystemSignature<AiControllerSystem>(signature);
}

void AiControllerSystem::Update()
{
	for (Entity entity : m_Entities)
	{
		auto* const pAiControllerComp = m_pRegistry->GetComponent<AiControllerComponent>(entity);
		auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);

		if (!pAiControllerComp->IsActive())
		{
			continue;
		}

		if (!pMoveComp->bCanMove)
		{
			continue;
		}

		if (pAiControllerComp->MoveTimer < pAiControllerComp->MOVE_INTERVAL)
		{
			pAiControllerComp->MoveTimer += Timer::GetInstance().GetDeltaTime();
		}
		else 
		{
			ChooseDirection(pMoveComp);
			pAiControllerComp->MoveTimer = 0;
		}
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