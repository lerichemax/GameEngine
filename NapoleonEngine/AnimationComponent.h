#pragma once
#include "Component.h"

struct AnimationComponent final : public ecs::Component
{
	PROPERTY(std::vector<std::string>, AnimationSprites);
	PROPERTY(float, TimePerSprite);

	int CurrentSpriteIndex;
};

SERIALIZE_CLASS(AnimationComponent, ecs::Component)
