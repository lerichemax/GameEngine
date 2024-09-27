#include "PCH.h"
#include "WrongWayJumper.h"
#include "WrongWay.h"

#include "GameObject.h"
#include "Timer.h"

WrongWayJumper::WrongWayJumper()
	:JumperSystem()
{
}

//void WrongWayJumper::Jump(glm::vec2 const& startPos, glm::vec2 const& targetPos)
//{
//	m_bIsJumping = true;
//	m_bJumpDown = false;
//
//	m_TargetPos = targetPos;
//	m_Halfway = startPos;
//
//	glm::vec2 const jumpDir = targetPos - startPos;
//	float const jumpDist = glm::length(jumpDir);
//	auto const dirNorm = glm::normalize(jumpDir);
//
//	m_Halfway += (dirNorm * (jumpDist / 2));
//
//	if (GetGameObject()->HasComponent<WrongWay>())
//	{
//		//m_Halfway.x += m_pGameObject->GetComponent<WrongWay>()->IsEscheresqueRight() ? JUMP_MAX_HEIGHT : -JUMP_MAX_HEIGHT;
//	}
//	
//}
//
//void WrongWayJumper::UpdateFall()
//{
//	auto pTransform = GetGameObject()->GetTransform();
//	auto pos = pTransform->GetLocation();
//	glm::vec2 dir{};
//
//	if (!m_bJumpDown)
//	{
//		dir = m_Halfway - pos;
//	}
//	else
//	{
//		if (GetGameObject()->HasComponent<WrongWay>())
//		{
//			//pTransform->SetWorldPosition(
//			//	pos.x += (m_pGameObject->GetComponent<WrongWay>()->IsEscheresqueRight() ? -FALL_SPEED : FALL_SPEED) 
//			//	* Timer::Get().GetDeltaTime(), pos.y );
//		}
//		
//		m_FallTime += Timer::Get().GetDeltaTime();
//		//if (m_FallTime >= FALL_TIME)
//		//{
//		//	
//		//}
//		return;
//	}
//
//	dir = glm::normalize(dir);
//
//	pTransform->Translate(pos + dir * JUMP_SPEED * Timer::Get().GetDeltaTime());
//
//	if (glm::length(pTransform->GetLocation() - m_Halfway) <= 2.f)
//	{
//		m_bJumpDown = true;
//	}
//}