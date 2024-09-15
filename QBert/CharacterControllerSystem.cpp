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

		if (InputManager::GetInstance().IsUp(SDL_SCANCODE_D))
		{
			pCharacterMovement->CurrentDirection = ConnectionDirection::downRight;
		}
		else if (InputManager::GetInstance().IsUp(SDL_SCANCODE_S))
		{
			pCharacterMovement->CurrentDirection = ConnectionDirection::downLeft;
		} 
		else if (InputManager::GetInstance().IsUp(SDL_SCANCODE_A)) // fix azerty
		{
			pCharacterMovement->CurrentDirection = ConnectionDirection::upLeft;
		}
		else if (InputManager::GetInstance().IsUp(SDL_SCANCODE_W)) // fix azerty
		{
			pCharacterMovement->CurrentDirection = ConnectionDirection::upRight;
		}
	}
}

void CharacterControllerSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(CharacterControllerSystem)).hash_code()));
}

void CharacterControllerSystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<CharacterControllerComponent>());
	signature.set(m_pRegistry->GetComponentType<MovementComponent>());

	m_pRegistry->SetSystemSignature<CharacterControllerSystem>(signature);
}