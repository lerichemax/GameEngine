#include "PCH.h"
#include "CharacterMovementSystem.h"

#include "Qube.h"

#include "TextureRendererComponent.h"
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
		m_pJumper->OnJumpLanded.Subscribe([this](){
			JumpToCurrentQube();
			});
	}
}

void CharacterMovementSystem::Update(ComponentManager* const pComponentManager)
{
	for (Entity entity : m_Entities)
	{
		auto* const pMoveComp = pComponentManager->GetComponent<MovementComponent>(entity);
		auto* const pRenderer = pComponentManager->GetComponent<TextureRendererComponent>(entity);

		if (!pMoveComp->IsActive() || !pMoveComp->m_CanMove)
		{
			continue;
		}

		Move(pRenderer, pMoveComp);
		pMoveComp->m_CurentDirection = ConnectionDirection::null;
	}
}

void CharacterMovementSystem::Move(TextureRendererComponent* const pRenderer, MovementComponent* const pMoveComp)
{
	if (pMoveComp->m_CurentDirection == ConnectionDirection::null)
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

	//SetDirectionTextures(direction);
	//
	//if (m_pCurrentQube->HasConnection(m_CurentDirection))
	//{
	//	SetJumpTexture(pRenderer, pMoveComp);
	//	OnMoveStarted.Notify();
	//	m_pCurrentQube->CharacterJumpOut();
	//	m_pCurrentQube = m_pCurrentQube->GetConnection(m_CurentDirection);
	//	m_pJumper->Jump(GetGameObject()->GetTransform()->GetPosition(), m_pCurrentQube->GetCharacterPos());
	//	p_MoveComp->m_CanMove = false;
	//	//SwitchState(new JumpingState(this, m_pJumper));
	//}
	//else
	//{
	//	float dist{};
	//	if (m_CurentDirection == ConnectionDirection::downLeft || m_CurentDirection == ConnectionDirection::upLeft)
	//	{
	//		dist = -25.f;
	//	}
	//	else if (m_CurentDirection == ConnectionDirection::downRight || m_CurentDirection == ConnectionDirection::upRight)
	//	{
	//		dist = 25.f;
	//	}

	//	m_pCurrentQube->CharacterJumpOut();
	//	//m_pJumper->JumpToDeath(GetGameObject()->GetTransform()->GetPosition(), dist);
	//}
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

void CharacterMovementSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<TextureRendererComponent>());
	signature.set(pRegistry->GetComponentType<MovementComponent>());
	signature.set(pRegistry->GetComponentType<JumpComponent>());

	pRegistry->SetSystemSignature<CharacterMovementSystem>(signature);
}

void CharacterMovementSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CharacterMovementSystem).name());
}

void CharacterMovementSystem::JumpToCurrentQube()
{
	//if (m_pCurrentQube == nullptr)
	//{
	//	return;
	//}

	MoveToCurrentQube();

	//temp ?
	//m_pCurrentQube->HandleQBertLanding();
	//m_CanMove = true;
}

void CharacterMovementSystem::MoveToCurrentQube()
{
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
	//	GetGameObject()->GetTransform()->Translate(m_pCurrentQube->GetCharacterPos());
	//}

	//SetIdleTexture();
	//m_CanMove = true;
}

void CharacterMovementSystem::SetIdleTexture(TextureRendererComponent* const pRenderer, MovementComponent* const pMoveComp)
{
	if (pRenderer == nullptr)
	{
		return;
	}

	switch (pMoveComp->m_CurentDirection)
	{
	case ConnectionDirection::downLeft:
		pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->m_TextureIdleDownLeft);
		break;
	case ConnectionDirection::downRight:
		pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->m_TextureIdleDownRight);
		break;
	case ConnectionDirection::upLeft:
		pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->m_TextureIdleUpLeft);
		break;
	case ConnectionDirection::upRight:
		pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->m_TextureIdleUpRight);
		break;
	}
}

void CharacterMovementSystem::SetJumpTexture(TextureRendererComponent* const pRenderer, MovementComponent* const pMoveComp)
{
	if (pRenderer == nullptr)
	{
		return;
	}

	switch (pMoveComp->m_CurentDirection)
	{
	case ConnectionDirection::downLeft:
		pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->m_TextureJumpDownLeft);
		break;
	case ConnectionDirection::downRight:
		pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->m_TextureJumpDownRight);
		break;
	case ConnectionDirection::upLeft:
		pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->m_TextureJumpUpLeft);
		break;
	case ConnectionDirection::upRight:
		pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(pMoveComp->m_TextureJumpUpRight);
		break;
	}
}