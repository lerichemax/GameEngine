#include "PCH.h"
#include "Coily.h"

#include "CoilyCharacterController.h"
#include "Qube.h"

#include "GameObject.h"
#include "QBert.h"
#include "ResourceManager.h"
#include "RendererComponent.h"
#include "FallingState.h"
#include "JumpingState.h"

#include "GameManager.h"
#include "SoundServiceLocator.h"
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
	if (m_pGameObject->HasComponent<CoilyCharacterController>())
	{
		m_pController = m_pGameObject->GetComponent<CoilyCharacterController>();
	}
	
	m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Egg_Small.png");
	m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Egg_Big.png");
	Enemy::Initialize();
}

void Coily::Move(ConnectionDirection direction)
{
	if (m_pController != nullptr && !m_pController->IsEnable() &&
		!m_pCurrentQube->HasConnection(direction))
	{
		FallSound();
		JumpToDeath(direction);
		SwitchState(new FallingState(this, m_pJumper));
		return;
	}
	
	if (m_bIsTransformed)
	{
		SetDirectionTextures(direction);
	}

	Enemy::Move(direction);
	if (m_pController != nullptr && !m_pController->IsEnable())
	{
		SwitchState(new JumpingState(this, m_pJumper));
	}
}

void Coily::FallSound() const
{
	SoundServiceLocator::GetService().Play(m_FallSoundId, 50);
}


void Coily::MeetCharacter(Character* pOther)
{
	if (!m_bIsTransformed)
	{
		return;
	}
	else
	{
		if (pOther->GetType() == CharacterType::player)
		{
			auto qbert = static_cast<QBert*>(pOther);
			qbert->Swear();
			qbert->Die();
		}
	}
}

void Coily::Die()
{
	if (m_pController != nullptr && m_pController->IsEnable())
	{
		Enemy::Die();
	}
	m_pGameObject->Notify((int)GameEvent::CoilyDies);
	m_pGameObject->Notify((int)VersusGameEvent::Player2Died);
}

void Coily::Transform(bool isTransformed)
{
	m_bIsTransformed = isTransformed;
	if (m_bIsTransformed)
	{
		m_pGameObject->Notify((int)VersusGameEvent::CoilyTransform);
	}
	else
	{
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Egg_Small.png");
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Egg_Big.png");
	}
}

void Coily::SetDirectionTextures(ConnectionDirection dir)
{
	if (m_FacingDirection == dir)
	{
		return;
	}

	m_FacingDirection = dir;

	switch (dir)
	{
	case ConnectionDirection::downLeft:
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Small_DownLeft.png");
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Big_DownLeft.png");
		break;
	case ConnectionDirection::downRight:
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Small_DownRight.png");
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Big_DownRight.png");
		break;
	case ConnectionDirection::upLeft:
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Small_UpLeft.png");
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Big_DownLeft.png");
		break;
	case ConnectionDirection::upRight:
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Small_UpRight.png");
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Big_UpRight.png");
		break;
	}
}

void Coily::MoveToCurrentQube()
{
	if (m_pController != nullptr && m_bIsTransformed)
	{
		m_pController->FindQBert();
	}
	Enemy::MoveToCurrentQube();
}
