#include "PCH.h"

#include "CharacterControllerSystem.h"
#include "CharacterMovementSystem.h"

#include "CharacterControllerComponent.h"
#include "MovementComponent.h"
#include "QubeComponent.h"

#include "InputManager.h"

void CharacterControllerSystem::Update()
{
	auto view = m_pRegistry->GetView<CharacterControllerComponent, MovementComponent>();
	for (Entity entity : view)
	{
		auto* const pCharacterControl = m_pRegistry->GetComponent<CharacterControllerComponent>(entity);

		if (!pCharacterControl->IsActive() || pCharacterControl->m_BlockInput)
		{
			continue;
		}

		auto* const pCharacterMovement = m_pRegistry->GetComponent<MovementComponent>(entity);
		
		if (!pCharacterMovement->bCanMove)
		{
			continue;
		}

		if (InputManager::Get().IsUp(pCharacterControl->Right))
		{
			pCharacterMovement->CurrentDirection = ConnectionDirection::downRight;
		}
		else if (InputManager::Get().IsUp(pCharacterControl->Down))
		{
			pCharacterMovement->CurrentDirection = ConnectionDirection::downLeft;
		} 
		else if (InputManager::Get().IsUp(pCharacterControl->Left)) // fix azerty
		{
			pCharacterMovement->CurrentDirection = ConnectionDirection::upLeft;
		}
		else if (InputManager::Get().IsUp(pCharacterControl->Up)) // fix azerty
		{
			pCharacterMovement->CurrentDirection = ConnectionDirection::upRight;
		}
	}
}

