#include "PCH.h"
#include "Coily.h"

#include "CoilyCharacterController.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "RendererComponent.h"
#include "FallingState.h"
#include "JumpingState.h"

#include "GameManagerSystem.h"
#include "VersusGameManager.h"

Coily::Coily(unsigned int fallSoundId)
	: Enemy(500, CharacterType::coily),
	m_bIsTransformed(false),
	m_pController(nullptr),
	m_FallSoundId(fallSoundId)
{}

Coily::Coily(Coily const& other)
	: Enemy(other),
	m_bIsTransformed(other.IsTransformed()),
	m_pController(nullptr),
	m_FallSoundId(other.m_FallSoundId)
{
}

void Coily::Initialize()
{
	//if (GetEntity()->HasComponent<CoilyCharacterController>())
	//{
	//	//m_pController = m_pGameObject->GetComponent<CoilyCharacterController>();
	//}
	
	//m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Egg_Small.png");
	//m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Egg_Big.png");
	//Enemy::Initialize();
}

void Coily::DoMove(ConnectionDirection direction)
{
	//if (m_pController != nullptr /*&& !m_pController->IsEnable()*/ &&
	//	!m_pCurrentQube->HasConnection(direction))
	//{
	//	FallSound();
	//	JumpToDeath(direction);
	//	SwitchState(new FallingState(this, m_pJumper));
	//	return;
	//}

	Enemy::DoMove(direction);
	if (m_pController != nullptr /*&& !m_pController->IsEnable()*/)
	{
		SwitchState(new JumpingState(this, m_pJumper));
	}
}

void Coily::FallSound() const
{
	//SoundServiceLocator::GetService().Play(m_FallSoundId, 50);
}


void Coily::MeetCharacter(Characters* pOther)
{
	if (!m_bIsTransformed)
	{
		return;
	}
	else
	{
		//if (pOther->GetType() == CharacterType::player)
		//{
		//	//auto qbert = dynamic_cast<QBert*>(pOther);
		//	//qbert->Swear();
		//	//qbert->Die();
		//}
	}
}

void Coily::DoDie()
{
	if (m_pController != nullptr/* && m_pController->IsEnable()*/)
	{
		Enemy::DoDie();
		//GetEntity()->Notify(static_cast<int>(GameEvent::CoilyDies));
	}
	
	//GetEntity()->Notify(static_cast<int>(VersusGameEvent::Player2Died));
}

void Coily::Transform(bool isTransformed)
{
	m_bIsTransformed = isTransformed;
	if (m_bIsTransformed)
	{
		//GetEntity()->Notify(static_cast<int>(VersusGameEvent::CoilyTransform));
	}
	//else
	//{
	//	m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Egg_Small.png");
	//	m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Egg_Big.png");
	//}
}

void Coily::SetDirectionTextures(ConnectionDirection dir)
{
	if (!m_bIsTransformed || m_FacingDirection == dir)
	{
		return;
	}

	m_FacingDirection = dir;

	//switch (dir)
	//{
	//case ConnectionDirection::downLeft:
	//	m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Small_DownLeft.png");
	//	m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Big_DownLeft.png");
	//	break;
	//case ConnectionDirection::downRight:
	//	m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Small_DownRight.png");
	//	m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Big_DownRight.png");
	//	break;
	//case ConnectionDirection::upLeft:
	//	m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Small_UpLeft.png");
	//	m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Big_DownLeft.png");
	//	break;
	//case ConnectionDirection::upRight:
	//	m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Small_UpRight.png");
	//	m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Big_UpRight.png");
	//	break;
	//}
}

void Coily::MoveToCurrentQube()
{
	if (m_pController != nullptr && m_bIsTransformed)
	{
		m_pController->FindQBert();
	}
	Enemy::MoveToCurrentQube();
}
