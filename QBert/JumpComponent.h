#pragma once
#include "Component.h"

struct JumpComponent : public Component
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

	void Serialize(StreamWriter& writer) const override;

	void Reset();
};
