#include "PCH.h"
#include "PlayerControllerSystem.h"

#include "PlayerComponent.h"

#include "InputManager.h"
#include "Timer.h"

void PlayerControllerSystem::Update()
{
	auto view = m_pRegistry->GetView<TransformComponent, PlayerComponent>();

	for (Entity entity : view)
	{
		auto pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
		auto pPlayer = m_pRegistry->GetComponent<PlayerComponent>(entity);

		int moveValue = 0;

		if(InputManager::Get().IsHeldDown(SDL_SCANCODE_A))
		{
			moveValue = -1;
		}
		else if (InputManager::Get().IsHeldDown(SDL_SCANCODE_D))
		{
			moveValue = 1;
		}

		if (InputManager::Get().IsUp(SDL_SCANCODE_SPACE))
		{
			if (pPlayer->ShootTimer <= 0)
			{
 				Instantiate("PlayerProjectile", m_pRegistry->GetComponentInChildren<TransformComponent>(entity)->GetLocation());
				pPlayer->ShootTimer = pPlayer->ShootDelay;
			}
		}

		pPlayer->ShootTimer -= TimerLocator::Get()->GetDeltaTime();
		glm::vec2 location = pTransform->GetLocation();
		pTransform->SetLocation(location.x + (moveValue * pPlayer->MoveSpeed * TimerLocator::Get()->GetDeltaTime()), location.y);
	}
}