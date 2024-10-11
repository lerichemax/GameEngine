#pragma once
#include "Component.h"

struct UiManagerComponent : public Component
{
	Entity PointsCounterTextEntity{ NULL_ENTITY };
	Entity LivesCounterTextEntity{ NULL_ENTITY };

	Entity PointsCounterTextEntityP2{ NULL_ENTITY };
	Entity LivesCounterTextEntityP2{ NULL_ENTITY };

	Entity PauseMenuEntity{ NULL_ENTITY };
	Entity GameOverMenuEntity{ NULL_ENTITY };

	void Serialize(StreamWriter& writer) const override;
};