#include "PCH.h"
#include "JumperSystem.h"

#include "JumpComponent.h"

#include "Timer.h"

void JumperSystem::Update()
{
	for (Entity entity : m_Entities)
	{
		auto* const pJumpComp = m_pCompManager->GetComponent<JumpComponent>(entity);
		auto* const pTransform = m_pCompManager->GetComponent<ECS_TransformComponent>(entity);

		if (!pJumpComp->m_bIsFalling && (pJumpComp->m_bIsJumping || pJumpComp->m_bJumpDown))
		{
			UpdateJump(pJumpComp, pTransform);
		}
		else if (pJumpComp->m_bIsFalling)
		{
			UpdateFall(pJumpComp, pTransform);
		}
	}
}

void JumperSystem::Jump(glm::vec2 const& startPos, glm::vec2 const& targetPos, JumpComponent* const pJump)
{
	pJump->m_bIsJumping = true;
	pJump->m_bJumpDown = false;

	pJump->m_TargetPos = targetPos;
	pJump->m_Halfway = startPos;
	
	//if (GetGameObject()->HasComponent<WrongWay>() )
	//{
	//	glm::vec2 const jumpDir = targetPos - startPos;
	//	float const jumpDist = glm::length(jumpDir);
	//	auto const dirNorm = glm::normalize(jumpDir);
	//	
	//	m_Halfway += (dirNorm * (jumpDist/2));
	//	
	//	m_Halfway.x  += 50;
	//}
	//else
	//{
		pJump->m_Halfway.x += (targetPos.x - startPos.x) * 0.25f;
		if (targetPos.y < startPos.y)
		{
			pJump->m_Halfway.y -= pJump->JUMP_MAX_HEIGHT * 2;
		}
		else
		{
			pJump->m_Halfway.y -= pJump->JUMP_MAX_HEIGHT;
		}
	//}
}

void JumperSystem::JumpToDeath(glm::vec2 const& startPos, float xDist, JumpComponent* const pJump)
{
	pJump->m_bIsJumping = true;
	pJump->m_bJumpDown = false;
	pJump->m_bIsFalling = true;

	pJump->m_FallTime = 0;
	pJump->m_Halfway = startPos;
	pJump->m_Halfway.x += xDist;
	pJump->m_Halfway.y -= pJump->JUMP_MAX_HEIGHT *2;
	OnJumpedToDeath.Notify();
}

void JumperSystem::UpdateJump(JumpComponent* const pJump, ECS_TransformComponent* const pTransform)
{
	auto const pos = pTransform->GetPosition();
	glm::vec2 dir{};

	if (!pJump->m_bJumpDown)
	{
		dir = pJump->m_Halfway - pos;
	}
	else
	{
		dir = pJump->m_TargetPos - pos;
	}

	dir = glm::normalize(dir);

	pTransform->Translate(pos + dir * pJump->JUMP_SPEED * Timer::GetInstance().GetDeltaTime());

	if (glm::length(pTransform->GetPosition() - pJump->m_Halfway) <= 2.f)
	{
		pJump->m_bJumpDown = true;
	}
	else if (glm::length(pTransform->GetPosition() - pJump->m_TargetPos) <= 2.f)
	{
		pJump->m_bIsJumping = false;
		pJump->m_bJumpDown = false;
		OnJumpLanded.Notify();
	}
}

void JumperSystem::UpdateFall(JumpComponent* const pJump, ECS_TransformComponent* const pTransform)
{
	auto pos = pTransform->GetPosition();
	glm::vec2 dir{};

	if (!pJump->m_bJumpDown)
	{
		dir = pJump->m_Halfway - pos;
	}
	else
	{
		pTransform->Translate(pos.x, pos.y += pJump->FALL_SPEED * Timer::GetInstance().GetDeltaTime());
		pJump->m_FallTime += Timer::GetInstance().GetDeltaTime();
		if (pJump->m_FallTime >= pJump->FALL_TIME)
		{
			OnFell.Notify();
			pJump->m_bIsFalling = false;
			pJump->m_bIsJumping = false;
			pJump->m_bJumpDown = false;
		}
		return;
	}

	dir = glm::normalize(dir);

	pTransform->Translate(pos + dir * pJump->JUMP_SPEED * Timer::GetInstance().GetDeltaTime());

	if (glm::length(pTransform->GetPosition() - pJump->m_Halfway) <= 2.f)
	{
		pJump->m_bJumpDown = true;
	}
}

void JumperSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(JumperSystem).name());
}

void JumperSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<JumpComponent>());
	signature.set(pRegistry->GetComponentType<ECS_TransformComponent>());

	pRegistry->SetSystemSignature<JumperSystem>(signature);
}