#pragma once
#include "Component.h"

struct FPSCounter : public ecs::Component
{
	int m_FPS{};
};

SERIALIZE_CLASS(FPSCounter, ecs::Component)