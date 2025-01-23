#include "PCH.h"
#include "AnimationSystem.h"

#include "AnimationComponent.h"
#include "RendererComponent.h"

#include "ResourceManager.h"
#include "Timer.h"

void AnimationSystem::Update()
{
	auto view = m_pRegistry->GetView<AnimationComponent, RendererComponent>();

	for (Entity entity : view)
	{
		auto pAnimation = m_pRegistry->GetComponent<AnimationComponent>(entity);
		auto pRenderer = m_pRegistry->GetComponent<RendererComponent>(entity);

		if (pAnimation->SpriteTimer <= 0.f)
		{
			pAnimation->CurrentSpriteIndex++;
			if (pAnimation->CurrentSpriteIndex >= pAnimation->AnimationSprites.size())
			{
				pAnimation->CurrentSpriteIndex = 0;
			}

			pRenderer->pTexture = ResourceManager::Get().GetTexture(pAnimation->AnimationSprites[pAnimation->CurrentSpriteIndex]);
			pAnimation->SpriteTimer = pAnimation->TimePerSprite;
		}

		pAnimation->SpriteTimer -= TimerLocator::Get()->GetDeltaTime();
	}
}