#include "PCH.h"
#include "LifeSystem.h"

#include "LifeComponent.h"
#include "AnimationComponent.h"

void LifeSystem::Update()
{
	auto view = m_pRegistry->GetView<LifeComponent, AnimationComponent>();

	for (Entity entity : view)
	{
		auto pLife = m_pRegistry->GetComponent<LifeComponent>(entity);
		auto pAnimation = m_pRegistry->GetComponent<AnimationComponent>(entity);

		if (pLife->bIsHit)
		{
			if (!pLife->bIsDead)
			{
				//change animation to death animation
			}
			else if (pAnimation->IsAnimationFinished())
			{
				m_pRegistry->DestroyEntity(entity);
			}
		}
	}
}