#include "PCH.h"
#include "EnemySystem.h"

#include "EnemyInfoComponent.h"
#include "RendererComponent.h"
#include "EnemyGroupComponent.h"

#include "Texture2D.h"
#include "Timer.h"

void EnemySystem::Start()
{
	auto view = m_pRegistry->GetView<EnemyGroupComponent, EnemyInfoComponent>();
	Entity holdingEntity = m_pRegistry->FindComponentOfType<EnemyGroupComponent>()->GetEntity();
	for (Entity entity : view)
	{
		auto pGroup = m_pRegistry->GetComponent<EnemyGroupComponent>(entity);
		pGroup->MoveTimer = pGroup->MoveInterval;

		auto pEnemyInfos = m_pRegistry->GetComponents<EnemyInfoComponent>(entity);

		glm::vec2 startPosition{ -200,-150 };
		glm::vec2 currentPosition{ startPosition };
		float textHeight = 0;

		for (EnemyInfoComponent const* pInfo : pEnemyInfos)
		{
			for (size_t i = 0; i < pInfo->Rows; i++)
			{
				for (size_t j = 0; j < pInfo->AmountPerRows; j++)
				{
					auto pEnemyObject = Instantiate(pInfo->EnemyPrefab, currentPosition);
					currentPosition.x += pEnemyObject->GetComponent<RendererComponent>()->pTexture->GetWidth() * 2.f;
					textHeight = pEnemyObject->GetComponent<RendererComponent>()->pTexture->GetHeight() * 4.f;
					m_pRegistry->AddChild(holdingEntity, pEnemyObject->GetEntity());
				}
				currentPosition.x = startPosition.x;
				currentPosition.y += textHeight;
			}
		}
	}
}

void EnemySystem::Update()
{
	auto view = m_pRegistry->GetView<TransformComponent, EnemyGroupComponent>();

	for (Entity entity : view) // should be only one
	{
		auto pGroup = m_pRegistry->GetComponent<EnemyGroupComponent>(entity);
		auto pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

		if (pGroup->MoveTimer <= 0)
		{
			glm::vec2 currentLocation = pTransform->GetLocation();

			if (pGroup->CurrentDistance >= pGroup->MaxDistance)
			{
				pGroup->MoveDirection *= -1;
				pGroup->CurrentDistance -= pGroup->MaxDistance;
				currentLocation.y += pGroup->MoveDistance;
				if (pGroup->MoveInterval > pGroup->MinimumMoveInterval)
				{
					pGroup->MoveInterval -= pGroup->SpeedIncrease;
				}
			}
			else
			{
				currentLocation.x += pGroup->MoveDistance * pGroup->MoveDirection;
				pGroup->CurrentDistance += pGroup->MoveDistance;
			}
			pTransform->SetLocation(currentLocation);
			pGroup->MoveTimer = pGroup->MoveInterval;
		}
		pGroup->MoveTimer -= TimerLocator::Get()->GetDeltaTime();
	}
}