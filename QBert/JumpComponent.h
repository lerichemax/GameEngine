#pragma once
#include "Component.h"

struct JumpComponent : public Component
{
	const float JUMP_SPEED{ 200.f };
	const float JUMP_MAX_HEIGHT{ 17.f };
	const float FALL_SPEED{ 500.f };
	const float FALL_TIME{ 1.f };

	bool m_bJumpDown;
	bool m_bIsJumping;
	bool m_bIsFalling;

	float m_FallTime;

	glm::vec2 m_TargetPos;
	glm::vec2 m_Halfway;

	void Serialize(StreamWriter& writer) const override;
};
