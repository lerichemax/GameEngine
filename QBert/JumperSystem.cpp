#include "PCH.h"
#include "JumperSystem.h"

#include "JumpComponent.h"

#include "Timer.h"

void JumperSystem::Update()
{
	for (Entity entity : m_Entities)
	{
		auto* const pJumpComp = m_pRegistry->GetComponent<JumpComponent>(entity);

		if (!pJumpComp->m_bIsFalling && (pJumpComp->m_bIsJumping || pJumpComp->m_bJumpDown))
		{
			UpdateJump(entity);
		}
		else if (pJumpComp->m_bIsFalling)
		{
			UpdateFall(entity);
		}
	}
}

void JumperSystem::Jump(Entity entity, glm::vec2 const& startPos, glm::vec2 const& targetPos)
{
	auto* const pJump = m_pRegistry->GetComponent<JumpComponent>(entity);

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

void JumperSystem::JumpToDeath(Entity entity, glm::vec2 const& startPos, float xDist)
{
	auto* const pJump = m_pRegistry->GetComponent<JumpComponent>(entity);

	pJump->m_bIsJumping = true;
	pJump->m_bJumpDown = false;
	pJump->m_bIsFalling = true;

	pJump->m_FallTime = 0;
	pJump->m_Halfway = startPos;
	pJump->m_Halfway.x += xDist;
	pJump->m_Halfway.y -= pJump->JUMP_MAX_HEIGHT *2;
	OnJumpedToDeath.Notify(entity);
}

void JumperSystem::UpdateJump(Entity entity)
{
	auto* const pJump = m_pRegistry->GetComponent<JumpComponent>(entity);
	auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

	auto const pos = pTransform->GetLocation();
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

	if (glm::length(pTransform->GetLocation() - pJump->m_Halfway) <= 2.f)
	{
		pJump->m_bJumpDown = true;
	}
	else if (glm::length(pTransform->GetLocation() - pJump->m_TargetPos) <= 2.f)
	{
		pJump->m_bIsJumping = false;
		pJump->m_bJumpDown = false;
		OnJumpLanded.Notify(entity);
	}
}

void JumperSystem::UpdateFall(Entity entity)
{
	auto* const pJump = m_pRegistry->GetComponent<JumpComponent>(entity);
	auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

	auto pos = pTransform->GetLocation();
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
			OnFell.Notify(entity);
			pJump->m_bIsFalling = false;
			pJump->m_bIsJumping = false;
			pJump->m_bJumpDown = false;
		}
		return;
	}

	dir = glm::normalize(dir);

	pTransform->Translate(pos + dir * pJump->JUMP_SPEED * Timer::GetInstance().GetDeltaTime());

	if (glm::length(pTransform->GetLocation() - pJump->m_Halfway) <= 2.f)
	{
		pJump->m_bJumpDown = true;
	}
}

void JumperSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(JumperSystem)).hash_code()));
}

void JumperSystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<JumpComponent>());
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());

	m_pRegistry->SetSystemSignature<JumperSystem>(signature);
}