#include "PCH.h"
#include "CharacterMovementSystem.h"
#include "QubeSystem.h"

#include "QubeComponent.h"
#include "DiskComponent.h"
#include "RendererComponent.h"
#include "MovementComponent.h"
#include "JumpComponent.h"

#include "ResourceManager.h"
#include "JumperSystem.h"

#include "Coordinator.h"

void CharacterMovementSystem::Start()
{
	m_pJumper = GetSystem<JumperSystem>();

	if (m_pJumper != nullptr)
	{
		m_pJumper->OnJumpLanded.Subscribe([this](Entity entity){
			JumpToCurrentQube(entity);
			});
	}
}

void CharacterMovementSystem::Update()
{
	for (Entity entity : m_Entities)
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

	if (pMoveComp->CurrentDirection == ConnectionDirection::null)
	{
		return;
	}

	auto* pCurrentQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);
	pCurrentQube->bIsOccupied = false;

	pMoveComp->bCanMove = false;
	SetJumpTexture(entity);

	if (pCurrentQube->HasConnection(pMoveComp->CurrentDirection)) //does the current cube have a connection
	{
		auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
		OnMoveStarted.Notify(entity);

		pMoveComp->CurrentQube = pCurrentQube->GetConnection(pMoveComp->CurrentDirection);
		pCurrentQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);

		m_pJumper->Jump(entity, pTransform->GetLocation(), pCurrentQube->CharacterPos);
	}
	else if (pCurrentQube->ConnectionToDisk != EntityManager::NULL_ENTITY)
	{
		OnMoveStarted.Notify(entity);
		//m_pGameObject->GetComponent<BoxCollider>()->SetEnable(false);
		auto* const pDisk = m_pRegistry->GetComponent<DiskComponent>(pCurrentQube->ConnectionToDisk);
		auto* const pDiskTransform = m_pRegistry->GetComponent<TransformComponent>(pCurrentQube->ConnectionToDisk);
		auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

		pDisk->bHasQbert = true;
		pTransform->Translate(pDiskTransform->GetLocation());
		pTransform->SetParent(pDiskTransform);

		pMoveComp->CurrentQube = EntityManager::NULL_ENTITY;
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

		//pCurrentQube->CharacterJumpOut();
		m_pJumper->JumpToDeath(entity, pTransform->GetLocation(), dist);
	}
}

//void CharacterMovementSystem::SetCurrentQube(Qube* const pQube)
//{
//	if (pQube != nullptr)
//	{
//		//m_pCurrentQube = pQube;
//	}
//	
//	MoveToCurrentQube();
//}
//
//Qube* const CharacterMovementSystem::GetCurrentQube() const
//{
//	//return m_pCurrentQube;
//	return nullptr;
//}

void CharacterMovementSystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<RendererComponent>());
	signature.set(m_pRegistry->GetComponentType<MovementComponent>());
	signature.set(m_pRegistry->GetComponentType<JumpComponent>());

	m_pRegistry->SetSystemSignature<CharacterMovementSystem>(signature);
}

void CharacterMovementSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int>(std::type_index(typeid(CharacterMovementSystem)).hash_code()));
}

void CharacterMovementSystem::JumpToCurrentQube(Entity entity)
{
	auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);

	MoveToCurrentQube(entity);
	pMoveComp->bCanMove = true;
}

void CharacterMovementSystem::MoveToCurrentQube(Entity entity)
{
	auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);
	auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);

	//if (m_pCurrentQube == nullptr)
	//{
	//	return;
	//}

	//if (m_pCurrentQube->HasCharacter() && m_pCurrentQube->GetCharacter() != this)
	//{
	//	MeetCharacter(m_pCurrentQube->GetCharacter());
	//}

	//if (GetGameObject()->IsActive())
	//{
	//	//m_pCurrentQube->CharacterJumpIn(this);
	pTransform->Translate(pQube->CharacterPos);
	//}

	SetIdleTexture(entity);
	pMoveComp->CurrentDirection = ConnectionDirection::null;
	pMoveComp->bCanMove = true;
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
		 ResourceManager::GetInstance().TryGetTexture(pMoveComp->TextureIdleDownLeft, pRenderer->pTexture);
		break;
	case ConnectionDirection::downRight:
		ResourceManager::GetInstance().TryGetTexture(pMoveComp->TextureIdleDownRight, pRenderer->pTexture);
		break;
	case ConnectionDirection::upLeft:
		ResourceManager::GetInstance().TryGetTexture(pMoveComp->TextureIdleUpLeft, pRenderer->pTexture);
		break;
	case ConnectionDirection::upRight:
		ResourceManager::GetInstance().TryGetTexture(pMoveComp->TextureIdleUpRight, pRenderer->pTexture);
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
		ResourceManager::GetInstance().TryGetTexture(pMoveComp->TextureJumpDownLeft, pRenderer->pTexture);
		break;
	case ConnectionDirection::downRight:	
		ResourceManager::GetInstance().TryGetTexture(pMoveComp->TextureJumpDownRight, pRenderer->pTexture);
		break;
	case ConnectionDirection::upLeft:
		ResourceManager::GetInstance().TryGetTexture(pMoveComp->TextureJumpUpLeft, pRenderer->pTexture);
		break;
	case ConnectionDirection::upRight:
		ResourceManager::GetInstance().TryGetTexture(pMoveComp->TextureJumpUpRight, pRenderer->pTexture);
		break;
	}
}