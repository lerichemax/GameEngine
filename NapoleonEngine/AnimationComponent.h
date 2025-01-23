#pragma once
#include "Component.h"

struct AnimationComponent final : public ecs::Component
{
	friend class AnimationSystem;
public:
	PROPERTY(std::vector<std::string>, AnimationSprites);
	PROPERTY(float, TimePerSprite);

private:
	int CurrentSpriteIndex = 0;
	float SpriteTimer = 0.f;
};

SERIALIZE_CLASS(AnimationComponent, ecs::Component)
