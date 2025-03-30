#pragma once
#include "Component.h"

class Animation;
struct AnimationComponent final : public ecs::Component
{
	friend class AnimationSystem;
public:
	AnimationComponent();

	PROPERTY(bool, bIsPaused);
	PROPERTY(float, Rate);

	bool IsAnimationFinished() const { return bAnimationFinished; }
	void SetAnimation(Animation* const pNewAnim);

private:
	PROPERTY(Animation*, pAnimation); // doe not have ownership

	int CurrentSpriteIndex = 0;
	float SpriteTimer = 0.f;
	bool bAnimationFinished = false;
	bool bInitialized = false;
};

SERIALIZE_CLASS(AnimationComponent, ecs::Component)
