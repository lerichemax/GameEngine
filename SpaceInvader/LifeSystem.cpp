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
		auto pAnimationComp = m_pRegistry->GetComponent<AnimationComponent>(entity);

		if (pLife->bIsHit)
		{
			if (!pLife->bIsDead)
			{
				pAnimationComp->SetAnimation(pLife->pDeathAnimation);
				pLife->bIsDead = true;
			}
			else if (pAnimationComp->IsAnimationFinished())
			{
				m_pRegistry->DestroyEntity(entity);
			}
		}
	}
}