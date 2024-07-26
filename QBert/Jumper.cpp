#include "PCH.h"
#include "Jumper.h"
#include "WrongWay.h"

#include "GameObject.h"
#include "Timer.h"

Jumper::Jumper()
	:m_bJumpDown(false),
	m_bIsJumping(false),
	m_bIsFalling(false),
	m_FallTime(0.f),
	m_TargetPos(),
	m_Halfway()
{
	
}

void Jumper::Update()
{
	if (!m_bIsFalling && (m_bIsJumping || m_bJumpDown))
	{
		UpdateJump();
	}
	else if(m_bIsFalling)
	{
		UpdateFall();
	}
}

void Jumper::UpdateJump()
{
	auto pTransform = m_pGameObject->GetTransform();
	auto const pos = pTransform->GetPosition();
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
	
	pTransform->Translate(pos + dir * JUMP_SPEED * Timer::GetInstance().GetDeltaTime());

	if (glm::length(pTransform->GetPosition() - m_Halfway) <= 2.f)
	{
		m_bJumpDown = true;
	}
	else if(glm::length(pTransform->GetPosition() - m_TargetPos) <= 2.f)
	{
		m_bIsJumping = false;
		m_bJumpDown = false;
		OnJumpLanded.Notify();
	}
}

void Jumper::UpdateFall()
{
	auto pTransform = m_pGameObject->GetTransform();
	auto pos = pTransform->GetPosition();
	glm::vec2 dir{};

	if (!m_bJumpDown)
	{
		dir = m_Halfway - pos;
	}
	else
	{
		pTransform->Translate(pos.x, pos.y += FALL_SPEED * Timer::GetInstance().GetDeltaTime());
		m_FallTime += Timer::GetInstance().GetDeltaTime();
		if (m_FallTime >= FALL_TIME)
		{
			OnFell.Notify();
			m_bIsFalling = false;
			m_bIsJumping = false;
			m_bJumpDown = false;
		}
		return;
	}

	dir = glm::normalize(dir);

	pTransform->Translate(pos + dir * JUMP_SPEED * Timer::GetInstance().GetDeltaTime());

	if (glm::length(pTransform->GetPosition() - m_Halfway) <= 2.f)
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
	
	if (m_pGameObject->HasComponent<WrongWay>() )
	{
		glm::vec2 const jumpDir = targetPos - startPos;
		float const jumpDist = glm::length(jumpDir);
		auto const dirNorm = glm::normalize(jumpDir);
		
		m_Halfway += (dirNorm * (jumpDist/2));
		
		m_Halfway.x  += 50;
	}
	else
	{
		m_Halfway.x += (targetPos.x - startPos.x) * 0.25f;
		if (targetPos.y < startPos.y)
		{
			m_Halfway.y -= JUMP_MAX_HEIGHT * 2;
		}
		else
		{
			m_Halfway.y -= JUMP_MAX_HEIGHT;
		}
	}
}

void Jumper::JumpToDeath(glm::vec2 const& startPos, float xDist)
{
	m_bIsJumping = true;
	m_bJumpDown = false;
	m_bIsFalling = true;

	m_FallTime = 0;
	m_Halfway = startPos;
	m_Halfway.x += xDist;
	m_Halfway.y -= JUMP_MAX_HEIGHT *2;
	OnJumpedToDeath.Notify();
}

void Jumper::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(Jumper).name());

	BehaviourComponent::Serialize(writer);
}