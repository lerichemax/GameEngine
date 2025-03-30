#include "PCH.h"
#include "AnimationComponent.h"

#include "Animation.h"

AnimationComponent::AnimationComponent()
	:pAnimation{ nullptr },
	bIsPaused{false},
	Rate{1.f}
{
}

void AnimationComponent::SetAnimation(Animation* const pNewAnim)
{
	pAnimation = pNewAnim;
	CurrentSpriteIndex = 0;
	SpriteTimer = 0.f;
	bAnimationFinished = false;
	bInitialized = false;
}