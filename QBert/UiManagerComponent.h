#pragma once
#include "Component.h"

struct UiManagerComponent : public ecs::Component
{
	Entity PointsCounterTextEntity{ NULL_ENTITY };
	Entity LivesCounterTextEntity{ NULL_ENTITY };

	Entity PointsCounterTextEntityP2{ NULL_ENTITY };
	Entity LivesCounterTextEntityP2{ NULL_ENTITY };

	Entity PauseMenuEntity{ NULL_ENTITY };
	Entity GameOverMenuEntity{ NULL_ENTITY };
};

SERIALIZE_CLASS(UiManagerComponent, ecs::Component)