#include "PCH.h"
#include "CollisionSystem.h"

#include "ColliderComponent.h"

void CollisionSystem::Update()
{
	auto view = m_pRegistry->GetView<ColliderComponent, TransformComponent>();

	for (Entity entity : view)
	{
		auto* const pCollider = m_pRegistry->GetComponent<ColliderComponent>(entity);
		if (!pCollider->IsActive())
		{
			continue;
		}

		auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

		pCollider->pShape->Pos = pTransform->GetLocation();

		if (pCollider->bDraw)
		{
			Debugger::Get().DrawDebugShape(pCollider->GetShape());
		}
	}

	for (Entity entity : view) // nested loop - > slow ? //TODO investigate more efficient algorythm
	{
		auto* const pCollider = m_pRegistry->GetComponent<ColliderComponent>(entity);
		if (!pCollider->IsActive())
		{
			continue;
		}

		for (Entity otherEntity : view)
		{
			if (entity == otherEntity)
			{
				continue;
			}

			auto* const pOtherCollider = m_pRegistry->GetComponent<ColliderComponent>(otherEntity);

			if (!pOtherCollider->IsActive())
			{
				continue;
			}

			if (pCollider->bIsTrigger)
			{
				HandleOverlapping(pCollider, pOtherCollider);
			}
			else
			{
				HandleCollision(pCollider, pOtherCollider);
			}
		}
	}
}

void CollisionSystem::HandleOverlapping(ColliderComponent* const pCollider, ColliderComponent* const pOtherCollider)
{
	bool bWasOverlapping = 
		std::find(pCollider->OverlappingColliders.begin(), pCollider->OverlappingColliders.end(), pOtherCollider->GetEntity()) != pCollider->OverlappingColliders.end();
	
	if (pCollider->pShape->IsOverlapping(pOtherCollider->GetShape()))
	{	
		if (!bWasOverlapping)
		{
			pCollider->TriggerEnter(pOtherCollider->GetEntity());
			pCollider->OverlappingColliders.insert(pOtherCollider->GetEntity());
		}
	}
	else if (bWasOverlapping)
	{
		pCollider->OverlappingColliders.erase(pOtherCollider->GetEntity());
	}
}

void CollisionSystem::HandleCollision(ColliderComponent* const pCollider, ColliderComponent* const pOtherCollider)
{
	//TODO

	//	else if (!bWasOverlapping && !pCollider->bIsTrigger)
	//	{
	//		pCollider->TriggerExit(pOtherCollider->GetEntity()->GetEntity());
	//}
}
