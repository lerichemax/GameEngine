#include "PCH.h"
#include "Jumper.h"

#include "Timer.h"

void Jumper::UpdateJump(empire::TransformComponent* transform)
{
	auto pos = transform->GetPosition();
	glm::vec2 dir{};
	
	if (!m_bJumpDown)
	{
		dir = m_Halfway - pos;
	}
	else
	{
		dir = m_TargetPos - pos;
	}
	
	dir = glm::normalize(dir);
	
	transform->Translate(pos + dir * JUMP_SPEED * empire::Timer::GetInstance().GetDeltaTime());

	if (glm::length(transform->GetPosition() - m_Halfway) <= 2.f)
	{
		m_bJumpDown = true;
	}
	else if(glm::length(transform->GetPosition() - m_TargetPos) <= 2.f)
	{
		m_bIsJumping = false;
	}
}

void Jumper::UpdateFall(empire::TransformComponent* transform)
{
	auto pos = transform->GetPosition();
	glm::vec2 dir{};

	if (!m_bJumpDown)
	{
		dir = m_Halfway - pos;
	}
	else
	{
		transform->Translate(pos.x, pos.y += FALL_SPEED * empire::Timer::GetInstance().GetDeltaTime());
		m_FallTime += empire::Timer::GetInstance().GetDeltaTime();
		if (m_FallTime >= FALL_TIME)
		{
			m_bIsDead = true;
		}
		return;
	}

	dir = glm::normalize(dir);

	transform->Translate(pos + dir * JUMP_SPEED * empire::Timer::GetInstance().GetDeltaTime());

	if (glm::length(transform->GetPosition() - m_Halfway) <= 2.f)
	{
		m_bJumpDown = true;
	}
}


void Jumper::Jump(glm::vec2 const& startPos, glm::vec2 const& targetPos)
{
	m_bIsJumping = true;
	m_bJumpDown = false;

	m_TargetPos = targetPos;
	m_Halfway = startPos;
	m_Halfway.x += (targetPos.x - startPos.x) * 0.25f;
	if (targetPos.y < startPos.y)
	{
		m_Halfway.y -= JUMP_MAX_HEIGHT *2;
	}
	else
	{
		m_Halfway.y -= JUMP_MAX_HEIGHT;
	}
}

void Jumper::JumpToDeath(glm::vec2 const& startPos, float xDist)
{
	m_bIsJumping = true;
	m_bJumpDown = false;

	m_FallTime = 0;
	m_Halfway = startPos;
	m_Halfway.x += xDist;
	m_Halfway.y -= JUMP_MAX_HEIGHT *2;
}