#include "PCH.h"
#include "Coily.h"

#include "CoilyCharacterController.h"
#include "Qube.h"

#include "GameObject.h"
#include "QBert.h"
#include "ResourceManager.h"
#include "RendererComponent.h"

#include "GameManager.h"
#include "VersusGameManager.h"

Coily::Coily()
	: Enemy(500),
	m_bIsTransformed(false),
	m_pController(nullptr)
{}

Coily::Coily(Coily const& other)
	: Enemy(other),
	m_bIsTransformed(other.IsTransformed()),
	m_pController(nullptr)
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
	//if (!m_pCurrentQube->HasConnection(direction))
	//{
	//	JumpToDeath(direction);
	//	return;
	//}
	
	if (m_bIsTransformed)
	{
		SetDirectionTextures(direction);
	}
	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pJumpText);
	Enemy::Move(direction);
}

void Coily::MeetCharacter(Character* pOther)
{
	if (!m_bIsTransformed)
	{
		return;
	}
	else
	{
		if (pOther->GetType() == Type::player)
		{
			static_cast<QBert*>(pOther)->Die();
		}
	}
}

void Coily::Die()
{
	if (m_pGameObject->GetComponent<CoilyCharacterController>()->IsEnable())
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
	if (m_pController != nullptr)
	{
		m_pController->FindQBert();
	}
	Enemy::MoveToCurrentQube();
}
