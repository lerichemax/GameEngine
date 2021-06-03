#include "PCH.h"
#include "WrongWayJumper.h"
#include "WrongWay.h"

#include "GameObject.h"
#include "Timer.h"

WrongWayJumper::WrongWayJumper()
	:Jumper()
{
}

void WrongWayJumper::Jump(glm::vec2 const& startPos, glm::vec2 const& targetPos)
{
	m_bIsJumping = true;
	m_bJumpDown = false;

	m_TargetPos = targetPos;
	m_Halfway = startPos;

	glm::vec2 jumpDir = targetPos - startPos;
	float jumpDist = glm::length(jumpDir);
	auto dirNorm = glm::normalize(jumpDir);

	m_Halfway += (dirNorm * (jumpDist / 2));

	if (m_pGameObject->HasComponent<WrongWay>())
	{
		m_Halfway.x += m_pGameObject->GetComponent<WrongWay>()->IsEscheresqueRight() ? JUMP_MAX_HEIGHT : -JUMP_MAX_HEIGHT;
	}
	
}

void WrongWayJumper::UpdateFall(empire::TransformComponent* transform)
{
	auto pos = transform->GetWorldPosition();
	glm::vec2 dir{};

	if (!m_bJumpDown)
	{
		dir = m_Halfway - pos;
	}
	else
	{
		if (m_pGameObject->HasComponent<WrongWay>())
		{
			transform->SetWorldPosition(
				pos.x += (m_pGameObject->GetComponent<WrongWay>()->IsEscheresqueRight() ? -FALL_SPEED : FALL_SPEED) 
				* empire::Timer::GetInstance().GetDeltaTime(), pos.y );
		}
		
		m_FallTime += empire::Timer::GetInstance().GetDeltaTime();
		if (m_FallTime >= FALL_TIME)
		{
			m_bIsDead = true;
		}
		return;
	}

	dir = glm::normalize(dir);

	transform->SetWorldPosition(pos + dir * JUMP_SPEED * empire::Timer::GetInstance().GetDeltaTime());

	if (glm::length(transform->GetWorldPosition() - m_Halfway) <= 2.f)
	{
		m_bJumpDown = true;
	}
}