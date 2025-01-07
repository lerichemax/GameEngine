#pragma once
#include "Component.h"

struct RotationComponent final : public ecs::Component
{
	float Angle;
	float Speed;
};
