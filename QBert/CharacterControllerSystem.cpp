#include "PCH.h"

#include "CharacterControllerSystem.h"
#include "CharacterMovementSystem.h"

#include "CharacterControllerComponent.h"
#include "MovementComponent.h"

#include "InputManager.h"

void CharacterControllerSystem::Start()
{
	//m_pCharacterMover = GetSystem<CharacterMovementSystem>();
}

void CharacterControllerSystem::Update(ComponentManager* const pComponentManager)
{
	for (Entity entity : m_Entities)
	{
		auto* const pCharacterControl = pComponentManager->GetComponent<CharacterControllerComponent>(entity);

		if (!pCharacterControl->IsActive() || pCharacterControl->m_BlockInput)
		{
			continue;
		}

		auto* const pCharacterMovement = pComponentManager->GetComponent<MovementComponent>(entity);

		if (InputManager::GetInstance().IsUp(SDL_SCANCODE_D))
		{
			pCharacterMovement->m_CurentDirection = ConnectionDirection::downRight;
		}
		else if (InputManager::GetInstance().IsUp(SDL_SCANCODE_S))
		{
			pCharacterMovement->m_CurentDirection = ConnectionDirection::downLeft;
		} 
		else if (InputManager::GetInstance().IsUp(SDL_SCANCODE_Q))
		{
			pCharacterMovement->m_CurentDirection = ConnectionDirection::upLeft;
		}
		else if (InputManager::GetInstance().IsUp(SDL_SCANCODE_Z))
		{
			pCharacterMovement->m_CurentDirection = ConnectionDirection::upRight;
		}
	}
}

void CharacterControllerSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CharacterControllerSystem).name());
}

void CharacterControllerSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<CharacterControllerComponent>());
	signature.set(pRegistry->GetComponentType<MovementComponent>());

	pRegistry->SetSystemSignature<CharacterControllerSystem>(signature);
}