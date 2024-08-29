#include "PCH.h"
#include "CharacterMovementSystem.h"
#include "QubeSystem.h"

#include "QubeComponent.h"

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
			pMoveComp->CurrentDirection = ConnectionDirection::null;
			continue;
		}

		Move(entity);
		pMoveComp->CurrentDirection = ConnectionDirection::null;
	}
}

void CharacterMovementSystem::Move(Entity entity)
{
	auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);

	if (pMoveComp->CurrentDirection == ConnectionDirection::null)
	{
		return;
	}

	//if (!m_pCurrentQube->HasConnection(direction) && !m_pCurrentQube->HasConnectionToDisk())
	//{
	//	JumpToDeath(direction);
	//	SwitchState(new FallingState(this, m_pJumper));
	//	SoundServiceLocator::GetService().Play(m_FallSoundID, 50);
	//	return;
	//}

	auto* pCurrentQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);
	pMoveComp->bCanMove = false;
	SetJumpTexture(entity);

	if (pCurrentQube->HasConnection(pMoveComp->CurrentDirection)) //does the current cube have a connection
	{
		auto* const pTransform = m_pRegistry->GetComponent<ECS_TransformComponent>(entity);
		OnMoveStarted.Notify();
		//pCurrentQube->CharacterJumpOut();
		pMoveComp->CurrentQube = pCurrentQube->GetConnection(pMoveComp->CurrentDirection);
		pCurrentQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);

		m_pJumper->Jump(entity, pTransform->GetPosition(), pCurrentQube->CharacterPos);
		
		//SwitchState(new JumpingState(this, m_pJumper));
	}
	else
	{
		auto* const pTransform = m_pRegistry->GetComponent<ECS_TransformComponent>(entity);
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
		m_pJumper->JumpToDeath(entity, pTransform->GetPosition(), dist);
	}
	//else if(m_pCurrentQube->HasConnectionToDisk())
	//{
	//	SoundServiceLocator::GetService().Play(m_JumpSoundID, 50);
	//	//m_pGameObject->GetComponent<BoxCollider>()->SetEnable(false);
	//	m_pCurrentQube->GetConnectedDisk()->ReceivePlayer(this);
	//	m_pCurrentQube->CharacterJumpOut();
	//	m_pCurrentQube = nullptr;
	//	m_bCanMove = false;
	//}
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

	//temp ?
	//m_pCurrentQube->HandleQBertLanding();
	pMoveComp->bCanMove = true;
}

void CharacterMovementSystem::MoveToCurrentQube(Entity entity)
{
	auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);
	auto* const pTransform = m_pRegistry->GetComponent<ECS_TransformComponent>(entity);
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
		pRenderer->pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->TextureIdleDownLeft);
		break;
	case ConnectionDirection::downRight:
		pRenderer->pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->TextureIdleDownRight);
		break;
	case ConnectionDirection::upLeft:
		pRenderer->pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->TextureIdleUpLeft);
		break;
	case ConnectionDirection::upRight:
		pRenderer->pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->TextureIdleUpRight);
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
		pRenderer->pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->TextureJumpDownLeft);
		break;
	case ConnectionDirection::downRight:
		pRenderer->pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->TextureJumpDownRight);
		break;
	case ConnectionDirection::upLeft:
		pRenderer->pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->TextureJumpUpLeft);
		break;
	case ConnectionDirection::upRight:
		pRenderer->pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->TextureJumpUpRight);
		break;
	}
}