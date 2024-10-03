#include "PCH.h"
#include "JumperSystem.h"

#include "JumpComponent.h"
#include "AiControllerComponent.h"
#include "QbertComponent.h"

#include "Timer.h"

void JumperSystem::Update()
{
	for (Entity entity : m_Entities)
	{
		auto* const pJumpComp = m_pRegistry->GetComponent<JumpComponent>(entity);

		if (!pJumpComp->IsActive())
		{
			continue;
		}

		if (!pJumpComp->bIsFalling && (pJumpComp->bIsJumping || pJumpComp->bJumpDown))
		{
			UpdateJump(entity);
		}
		else if (pJumpComp->bIsFalling)
		{
			UpdateFall(entity);
		}
	}
}

void JumperSystem::Jump(Entity entity, glm::vec2 const& startPos, glm::vec2 const& targetPos)
{
	auto* const pJump = m_pRegistry->GetComponent<JumpComponent>(entity);

	pJump->bIsJumping = true;
	pJump->bJumpDown = false;

	pJump->TargetPos = targetPos;

	glm::vec2 const jumpDir = targetPos - startPos;
	float const jumpDist = glm::length(jumpDir);
	auto const dirNorm = glm::normalize(jumpDir);

	pJump->Halfway = startPos + (dirNorm * (jumpDist / 2));
	
	if (targetPos.y < startPos.y)
	{
		pJump->Halfway += pJump->Direction * pJump->JUMP_MAX_HEIGHT;
	}
	else
	{
		pJump->Halfway += pJump->Direction * (pJump->JUMP_MAX_HEIGHT * 2);
	}
}

void JumperSystem::JumpToDeath(Entity entity, glm::vec2 const& startPos, float xDist)
{
	auto* const pJump = m_pRegistry->GetComponent<JumpComponent>(entity);

	pJump->bIsJumping = true;
	pJump->bJumpDown = false;
	pJump->bIsFalling = true;

	pJump->FallTime = 0;
	pJump->Halfway = startPos;
	pJump->Halfway.x += xDist;
	pJump->Halfway += pJump->Direction * (pJump->JUMP_MAX_HEIGHT * 2.f);
	OnJumpedToDeath.Notify(entity);
}

void JumperSystem::UpdateJump(Entity entity)
{
	auto* const pJump = m_pRegistry->GetComponent<JumpComponent>(entity);
	auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

	auto const Pos = pTransform->GetLocation();
	glm::vec2 dir{};

	if (!pJump->bJumpDown)
	{
		dir = pJump->Halfway - Pos;
	}
	else
	{
		dir = pJump->TargetPos - Pos;
	}

	dir = glm::normalize(dir);

	pTransform->Translate(Pos + dir * pJump->JUMP_SPEED * Timer::Get().GetDeltaTime());

	if (glm::length(pTransform->GetLocation() - pJump->Halfway) <= 2.f)
	{
		pJump->bJumpDown = true;
	}
	else if (glm::length(pTransform->GetLocation() - pJump->TargetPos) <= 2.f)
	{
		pJump->bIsJumping = false;
		pJump->bJumpDown = false;
		OnJumpLanded.Notify(entity);
	}
}

void JumperSystem::UpdateFall(Entity entity)
{
	auto* const pJump = m_pRegistry->GetComponent<JumpComponent>(entity);
	auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

	auto Pos = pTransform->GetLocation();

	if (!pJump->bJumpDown)
	{
		glm::vec2 dir = pJump->Halfway - Pos;
		dir = glm::normalize(dir);

		pTransform->Translate(Pos + dir * pJump->JUMP_SPEED * Timer::Get().GetDeltaTime());

		if (glm::length(pTransform->GetLocation() - pJump->Halfway) <= 2.f)
		{
			pJump->bJumpDown = true;
		}
	}
	else
	{
		pTransform->Translate(Pos - pJump->Direction * (pJump->FALL_SPEED * Timer::Get().GetDeltaTime()));
		pJump->FallTime += Timer::Get().GetDeltaTime();
		if (pJump->FallTime >= pJump->FALL_TIME)
		{
			OnFell.Notify(entity);
			pJump->bIsFalling = false;
			pJump->bIsJumping = false;
			pJump->bJumpDown = false;
		}
		return;
	}
}

void JumperSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(JumperSystem)).hash_code()));
}

void JumperSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<JumpComponent>());
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());

	m_pRegistry->SetSystemSignature<JumperSystem>(signature);
}