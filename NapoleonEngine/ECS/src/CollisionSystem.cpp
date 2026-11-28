#include "PCH.h"
#include "CollisionSystem.h"
#include "TransformSystem.h"

#include "ColliderComponent.h"

#include "Camera.h"

EventHandler<CollisionSystem, Entity, Entity> CollisionSystem::TriggerEnter{};
EventHandler<CollisionSystem, Entity, Entity> CollisionSystem::TriggerExit{};
EventHandler<CollisionSystem, Entity, Entity> CollisionSystem::Collision{};

CollisionSystem::~CollisionSystem()
{
	TriggerEnter.UnsuscribeAll();
	TriggerExit.UnsuscribeAll();
	Collision.UnsuscribeAll();
}

void CollisionSystem::Update()
{
	auto view = m_pRegistry->GetView<ColliderComponent, TransformComponent>();

	for (Entity entity : view)
	{
		auto* const pCollider = m_pRegistry->GetComponent<ColliderComponent>(entity);
		if (!pCollider->IsActive() || !IS_VALID(pCollider->GetShape()))
		{
			continue;
		}

		auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
		
		glm::mat3 transformed = CameraLocator::Get()->TransformIntoCameraSpace(pTransform->GetWorldTransformMatrix());
		glm::vec2 transformedLocation;
		TransformSystem::ExtractTranslation(transformed, transformedLocation);
		pCollider->GetShape()->Pos = pCollider->m_Offset + transformedLocation;

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

			if (pCollider->bDraw)
			{
				Debugger::Get().DrawDebugShape(pCollider->GetShape());
			}
		}
	}

	for (Entity entity : view)
	{
		auto* const pCollider = m_pRegistry->GetComponent<ColliderComponent>(entity);
		auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

		pCollider->m_PreviousLocation = pTransform->GetLocation();
	}
}

void CollisionSystem::HandleOverlapping(ColliderComponent* const pCollider, ColliderComponent* const pOtherCollider)
{
	bool bWasOverlapping = 
		std::find(pCollider->m_OverlappingColliders.begin(), pCollider->m_OverlappingColliders.end(), pOtherCollider->GetEntity()) != pCollider->m_OverlappingColliders.end();
	
	if (pCollider->m_pShape->IsOverlapping(pOtherCollider->GetShape()))
	{	
		if (!bWasOverlapping)
		{
			pCollider->TriggerEnter(pOtherCollider->GetEntity());
			TriggerEnter.Notify(pCollider->GetEntity(), pOtherCollider->GetEntity());
		}
	}
	else if (bWasOverlapping)
	{
		pCollider->TriggerExit(pOtherCollider->GetEntity());
		TriggerExit.Notify(pCollider->GetEntity(), pOtherCollider->GetEntity());
	}
}

void CollisionSystem::HandleCollision(ColliderComponent* const pCollider, ColliderComponent* const pOtherCollider)
{
	//TODO
	if (pCollider->m_pShape->IsOverlapping(pOtherCollider->GetShape()))
	{
		m_pRegistry->GetComponent<TransformComponent>(pCollider->GetEntity())->SetLocation(pCollider->m_PreviousLocation);
		m_pRegistry->GetComponent<TransformComponent>(pOtherCollider->GetEntity())->SetLocation(pOtherCollider->m_PreviousLocation);

		pCollider->Collide(pOtherCollider->GetEntity());
		pOtherCollider->Collide(pCollider->GetEntity());

		Collision.Notify(pCollider->GetEntity(), pOtherCollider->GetEntity());
	}
}
