#pragma once
#include "TransformComponent.h"
#include <glm/glm.hpp>

class Jumper final
{
public:
	Jumper() = default;
	void UpdateJump(empire::TransformComponent* transform);
	void UpdateFall(empire::TransformComponent* transform);

	void SetIsNotDead() { m_bIsDead = false; };
	
	void Jump(glm::vec2 const& startPos, glm::vec2 const& targetPos);
	void JumpToDeath(glm::vec2 const& startPos, float xDist);
	bool IsJumping() const { return m_bIsJumping; }
	bool IsDead() const { return m_bIsDead; }

private:
	const float JUMP_SPEED{ 200.f };
	const float JUMP_MAX_HEIGHT{ 17.f };
	const float FALL_SPEED{ 500.f };
	const float FALL_TIME{ 1.f };
	
	bool m_bJumpDown{false};
	bool m_bIsJumping{false};
	bool m_bIsDead{ false };

	float m_FallTime{};
	
	glm::vec2 m_TargetPos{};
	glm::vec2 m_Halfway{};
};
