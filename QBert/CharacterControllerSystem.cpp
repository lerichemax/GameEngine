#include "PCH.h"

#include "CharacterControllerSystem.h"
#include "CharacterMovementSystem.h"

#include "CharacterControllerComponent.h"
#include "MovementComponent.h"
#include "QubeComponent.h"

#include "InputManager.h"

void CharacterControllerSystem::Update()
{
	for (Entity entity : m_Entities)
	{
		auto* const pCharacterControl = m_pRegistry->GetComponent<CharacterControllerComponent>(entity);

		if (!pCharacterControl->IsActive() || pCharacterControl->m_BlockInput)
		{
			continue;
		}

		auto* const pCharacterMovement = m_pRegistry->GetComponent<MovementComponent>(entity);
		
		if (!pCharacterMovement->bCanMove)
		{
			return;
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

void CharacterControllerSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(CharacterControllerSystem)).hash_code()));
}

void CharacterControllerSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<CharacterControllerComponent>());
	signature.set(m_pRegistry->GetComponentType<MovementComponent>());

	m_pRegistry->SetSystemSignature<CharacterControllerSystem>(signature);
}