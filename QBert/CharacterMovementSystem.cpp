#include "PCH.h"
#include "CharacterMovementSystem.h"
#include "QubeSystem.h"
#include "JumperSystem.h"

#include "QubeComponent.h"
#include "DiskComponent.h"
#include "RendererComponent.h"
#include "MovementComponent.h"
#include "JumpComponent.h"
#include "AiControllerComponent.h"
#include "QbertComponent.h"

#include "ResourceManager.h"
#include "Registry.h"

void CharacterMovementSystem::Start()
{
	m_pJumper = m_pRegistry->GetSystem<JumperSystem>();

	if (m_pJumper != nullptr)
	{
		m_pJumper->OnJumpLanded.Subscribe([this](Entity entity){
			auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);
			pMoveComp->bCanMove = true;
			SetIdleTexture(entity);
			pMoveComp->CurrentDirection = ConnectionDirection::null;
			});
	}
}

void CharacterMovementSystem::Update()
{
	auto view = m_pRegistry->GetView<RendererComponent, MovementComponent, JumpComponent>();

	for (Entity entity : view)
	{
		auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);

		if (!pMoveComp->IsActive() || !pMoveComp->bCanMove)
		{
			continue;
		}

		Move(entity);
	}
}

void CharacterMovementSystem::Move(Entity entity)
{
	auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);

	MoveToCurrentQube(entity);

	if (pMoveComp->CurrentDirection == ConnectionDirection::null)
	{
		return;
	}

	auto* pCurrentQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);
	pCurrentQube->Characters.erase(entity);

	pMoveComp->bCanMove = false;
	SetJumpTexture(entity);

	if (pCurrentQube->HasConnectionForMovementMode(pMoveComp->Mode, pMoveComp->CurrentDirection)) //does the current cube have a connection
	{
		auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
		OnMoveStarted.Notify(entity);

		pMoveComp->CurrentQube = pCurrentQube->GetConnectionForMovementMode(pMoveComp->Mode, pMoveComp->CurrentDirection);
		pCurrentQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);

		m_pJumper->Jump(entity, pTransform->GetLocation(), pCurrentQube->GetEnemyTopPosition(pMoveComp->Mode));
	}
	else if (pCurrentQube->ConnectionToDisk->Direction != ConnectionDirection::null && !m_pRegistry->EntityHasTag(entity, ENEMY_TAG))
	{
		OnMoveStarted.Notify(entity);

		auto* const pDisk = m_pRegistry->GetComponent<DiskComponent>(pCurrentQube->ConnectionToDisk->Disk);
		auto* const pDiskTransform = m_pRegistry->GetComponent<TransformComponent>(pCurrentQube->ConnectionToDisk->Disk);
		auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

		m_pRegistry->GetComponent<QbertComponent>(entity)->Disk = pCurrentQube->ConnectionToDisk->Disk;

		pDisk->QbertEntity = entity;
		pTransform->SetLocation(pDiskTransform->GetLocation());
		pTransform->SetParent(pDiskTransform);

		pMoveComp->CurrentQube = NULL_ENTITY;

		OnJumpedOnDisk.Notify(pCurrentQube->ConnectionToDisk->Disk);
	}
	else
	{
		auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
		float dist{};
		if (pMoveComp->CurrentDirection == ConnectionDirection::downLeft || pMoveComp->CurrentDirection == ConnectionDirection::upLeft)
		{
			dist = -25.f;
		}
		else if (pMoveComp->CurrentDirection == ConnectionDirection::downRight || pMoveComp->CurrentDirection == ConnectionDirection::upRight)
		{
			dist = 25.f;
		}

		m_pJumper->JumpToDeath(entity, pTransform->GetLocation(), dist);
	}
}

void CharacterMovementSystem::MoveToCurrentQube(Entity entity)
{
	auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);
	auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);

	assert(IS_VALID(pQube) && "pQube is invalid");

	if (pQube->Characters.size() > 1)
	{
		OnMeetCharacter.Notify(entity, pQube->Characters);
	}

	pTransform->SetLocation(pQube->GetEnemyTopPosition(pMoveComp->Mode));
}

void CharacterMovementSystem::SetIdleTexture(Entity entity)
{
	auto* const pRenderer = m_pRegistry->GetComponent<RendererComponent>(entity);
	auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);

	if (pRenderer == nullptr)
	{
		return;
	}

	switch (pMoveComp->CurrentDirection)
	{
	case ConnectionDirection::downLeft:
		 ResourceManager::Get().TryGetTexture(pMoveComp->TextureIdleDownLeft, pRenderer->pTexture);
		break;
	case ConnectionDirection::downRight:
		ResourceManager::Get().TryGetTexture(pMoveComp->TextureIdleDownRight, pRenderer->pTexture);
		break;
	case ConnectionDirection::upLeft:
		ResourceManager::Get().TryGetTexture(pMoveComp->TextureIdleUpLeft, pRenderer->pTexture);
		break;
	case ConnectionDirection::upRight:
		ResourceManager::Get().TryGetTexture(pMoveComp->TextureIdleUpRight, pRenderer->pTexture);
		break;
	}
}

void CharacterMovementSystem::SetJumpTexture(Entity entity)
{
	auto* const pRenderer = m_pRegistry->GetComponent<RendererComponent>(entity);
	auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);

	if (pRenderer == nullptr)
	{
		return;
	}

	switch (pMoveComp->CurrentDirection)
	{
	case ConnectionDirection::downLeft:
		ResourceManager::Get().TryGetTexture(pMoveComp->TextureJumpDownLeft, pRenderer->pTexture);
		break;
	case ConnectionDirection::downRight:	
		ResourceManager::Get().TryGetTexture(pMoveComp->TextureJumpDownRight, pRenderer->pTexture);
		break;
	case ConnectionDirection::upLeft:
		ResourceManager::Get().TryGetTexture(pMoveComp->TextureJumpUpLeft, pRenderer->pTexture);
		break;
	case ConnectionDirection::upRight:
		ResourceManager::Get().TryGetTexture(pMoveComp->TextureJumpUpRight, pRenderer->pTexture);
		break;
	}
}