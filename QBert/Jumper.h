#pragma once
#include "TransformComponent.h"
#include <glm/glm.hpp>

class Jumper : public empire::Component
{
public:
	Jumper();
	Jumper* Clone() const override { return new Jumper(*this); }
	
	void UpdateJump(empire::TransformComponent* transform);
	virtual void UpdateFall(empire::TransformComponent* transform);

	void SetIsNotDead() { m_bIsDead = false; };
	
	virtual void Jump(glm::vec2 const& startPos, glm::vec2 const& targetPos);
	void JumpToDeath(glm::vec2 const& startPos, float xDist);
	bool IsJumping() const { return m_bIsJumping; }
	bool IsDead() const { return m_bIsDead; }

protected:
	void Update() override{};
	void Initialize() override{};

	const float JUMP_SPEED{ 200.f };
	const float JUMP_MAX_HEIGHT{17.f};
	const float FALL_SPEED{ 500.f };
	const float FALL_TIME{ 1.f };
	
	bool m_bJumpDown{false};
	bool m_bIsJumping{false};
	bool m_bIsDead{ false };

	float m_FallTime{};
	
	glm::vec2 m_TargetPos{};
	glm::vec2 m_Halfway{};

};
