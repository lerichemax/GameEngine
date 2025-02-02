#include "PCH.h"
#include "AnimationSystem.h"

#include "AnimationComponent.h"
#include "RendererComponent.h"

#include "ResourceManager.h"
#include "Timer.h"
#include "Animation.h"

void AnimationSystem::Update()
{
	auto view = m_pRegistry->GetView<AnimationComponent, RendererComponent>();

	for (Entity entity : view)
	{
		auto pAnimationComp = m_pRegistry->GetComponent<AnimationComponent>(entity);
		auto pRenderer = m_pRegistry->GetComponent<RendererComponent>(entity);

		auto pAnimation = pAnimationComp->pAnimation;
		if (!IS_VALID(pAnimation))
		{
			continue;
		}

		if (!pAnimationComp->bInitialized)
		{
			pRenderer->pTexture = ResourceManager::Get().GetTexture(pAnimation->m_AnimationSprites[pAnimationComp->CurrentSpriteIndex]);
			pAnimationComp->bInitialized = true;
			continue;
		}

		if (!pAnimationComp->bIsPaused && pAnimationComp->SpriteTimer >= pAnimation->m_TimePerSprite * pAnimationComp->Rate)
		{
			pAnimationComp->CurrentSpriteIndex++;
			if (pAnimationComp->CurrentSpriteIndex >= pAnimation->m_AnimationSprites.size())
			{
				if (pAnimation->m_bLoop)
				{
					pAnimationComp->CurrentSpriteIndex = 0;
				}
				else
				{
					pAnimationComp->bAnimationFinished = true;
					continue;
				}
			}

			pRenderer->pTexture = ResourceManager::Get().GetTexture(pAnimation->m_AnimationSprites[pAnimationComp->CurrentSpriteIndex]);
			pAnimationComp->SpriteTimer = 0.f;
		}

		pAnimationComp->SpriteTimer += TimerLocator::Get()->GetDeltaTime();
	}
}