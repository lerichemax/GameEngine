#pragma once
#include "BehaviourComponent.h"

#include "TransformComponent.h"
#include "Event.h"
#include <glm/glm.hpp>

class Jumper : public BehaviourComponent
{
public:
	Jumper();

	EventHandler<Jumper> OnJumpLanded;
	EventHandler<Jumper> OnFell;
	EventHandler<Jumper> OnJumpedToDeath;
	
	void UpdateJump();
	virtual void UpdateFall();

	virtual void Jump(glm::vec2 const& startPos, glm::vec2 const& targetPos);
	void JumpToDeath(glm::vec2 const& startPos, float xDist);
	bool IsJumping() const { return m_bIsJumping; }

	void Serialize(StreamWriter& writer) const override;

protected:
	void Update() override;

	const float JUMP_SPEED{ 200.f };
	const float JUMP_MAX_HEIGHT{17.f};
	const float FALL_SPEED{ 500.f };
	const float FALL_TIME{ 1.f };
	
	bool m_bJumpDown;
	bool m_bIsJumping;
	bool m_bIsFalling;

	float m_FallTime;
	
	glm::vec2 m_TargetPos;
	glm::vec2 m_Halfway;
};
