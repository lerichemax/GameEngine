#pragma once
#include "Component.h"

struct AnimationComponent final : public ecs::Component
{
	friend class AnimationSystem;
public:
	AnimationComponent();

	
	PROPERTY(bool, bLoop);
	PROPERTY(bool, bIsPaused);

	bool IsAnimationFinished() const { return bAnimationFinished; }

private:
	int CurrentSpriteIndex = 0;
	float SpriteTimer = 0.f;
	bool bAnimationFinished = false;
};

SERIALIZE_CLASS(AnimationComponent, ecs::Component)
