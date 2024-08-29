#pragma once
#include "Component.h"

struct UiManagerComponent : public Component
{
	Entity PointsCounterTextEntity;
	Entity LivesCounterTextEntity;

	Entity PauseMenuEntity;
	Entity GameOverMenuEntity;

	void Serialize(StreamWriter& writer) const override;
};