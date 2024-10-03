#include "PCH.h"
#include "CollisionSystem.h"

#include "ColliderComponent.h"

void CollisionSystem::Update()
{
	for (Entity entity : m_Entities) // slow
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

		for (Entity otherEntity : m_Entities)
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
		std::find(pCollider->OverlappingColliders.begin(), pCollider->OverlappingColliders.end(), pOtherCollider->GetGameObject()->GetEntity()) != pCollider->OverlappingColliders.end();
	
	if (pCollider->pShape->IsOverlapping(pOtherCollider->GetShape()))
	{	
		if (!bWasOverlapping)
		{
			pCollider->TriggerEnter(pOtherCollider->GetGameObject()->GetEntity());
			pCollider->OverlappingColliders.insert(pOtherCollider->GetGameObject()->GetEntity());
		}
	}
	else if (bWasOverlapping)
	{
		pCollider->OverlappingColliders.erase(pOtherCollider->GetGameObject()->GetEntity());
	}
}

void CollisionSystem::HandleCollision(ColliderComponent* const pCollider, ColliderComponent* const pOtherCollider)
{
	//TODO

	//	else if (!bWasOverlapping && !pCollider->bIsTrigger)
	//	{
	//		pCollider->TriggerExit(pOtherCollider->GetGameObject()->GetEntity());
	//}
}

void CollisionSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(CollisionSystem)).hash_code()));
}

void CollisionSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<ColliderComponent>());
	m_pRegistry->SetSystemSignature<CollisionSystem>(signature);
}