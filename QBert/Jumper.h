#pragma once
#include "TransformComponent.h"
#include <glm/glm.hpp>

class Jumper final
{
public:
	Jumper() = default;
	void Update(empire::TransformComponent* transform);

	void Jump(glm::vec3 const& startPos, glm::vec3 const& targetPos);

	bool IsJumping() const { return m_bIsJumping; }
private:
	const float JUMP_SPEED{ 200.f };
	const float JUMP_MAX_HEIGHT{ 17.f };

	bool m_bJumpDown{false};
	bool m_bIsJumping{false};
	
	glm::vec3 m_TargetPos;
	glm::vec3 m_Halfway;
	
};
