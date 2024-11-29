#pragma once
#include "Component.h"

struct JumpComponent : public ecs::Component
{
	const float JUMP_SPEED{ 200.f };
	const float JUMP_MAX_HEIGHT{ 17.f };
	const float FALL_SPEED{ 500.f };
	const float FALL_TIME{ 1.f };

	bool bJumpDown;
	bool bIsJumping;
	bool bIsFalling;

	float FallTime;

	glm::vec2 TargetPos;
	glm::vec2 Halfway;
	PROPERTY(glm::vec2, Direction);

	void Reset();
};

SERIALIZE_CLASS(JumpComponent, ecs::Component)