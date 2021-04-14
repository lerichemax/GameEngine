#include "PCH.h"
#include "Jumper.h"

#include "Timer.h"

void Jumper::Update(empire::TransformComponent* transform)
{
	auto pos = transform->GetPosition();
	glm::vec3 dir{};
	
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

void Jumper::Jump(glm::vec3 const& startPos, glm::vec3 const& targetPos)
{
	m_bIsJumping = true;
	m_bJumpDown = false;

	m_TargetPos = targetPos;
	m_Halfway = startPos;
	m_Halfway.x += (targetPos.x - startPos.x) * 0.25f;
	m_Halfway.y -= JUMP_MAX_HEIGHT;
	
}