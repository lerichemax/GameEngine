#include "PCH.h"
#include "ProjectileSystem.h"
#include "CollisionSystem.h"

#include "ProjectileComponent.h"

void ProjectileSystem::Initialize()
{
	CollisionSystem::TriggerEnter.Subscribe([this](Entity entityA, Entity entityB) {
		Entity projectileEntity = m_pRegistry->GetTag(entityA) == "Projectile" ? entityA : entityB;
		Entity otherEntity = projectileEntity == entityA ? entityB : entityA;
		m_pRegistry->DestroyEntity(projectileEntity);
		m_pRegistry->DestroyEntity(otherEntity);
		});
}

void ProjectileSystem::Update()
{
	auto view = m_pRegistry->GetView<TransformComponent, ProjectileComponent>();

	for (Entity entity : view)
	{
		auto pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
		auto pProjectile = m_pRegistry->GetComponent<ProjectileComponent>(entity);

		if (pTransform->GetLocation().y <= -220) // temp -> TODO : add camera function to detect if element is rendered on screen
		{
 			m_pRegistry->DestroyEntity(entity);
			continue;
		}

		glm::vec2 loc = pTransform->GetLocation();

		loc.y += pProjectile->MoveDirection * pProjectile->MoveSpeed * TimerLocator::Get()->GetDeltaTime();

		pTransform->SetLocation(loc);
	}
}