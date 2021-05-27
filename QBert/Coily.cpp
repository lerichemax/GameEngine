#include "PCH.h"
#include "Coily.h"
#include "Qube.h"

#include "GameObject.h"
#include "QBert.h"
#include "ResourceManager.h"
#include "RendererComponent.h"

#include <thread>

Coily::Coily()
	: Enemy(500),
	m_pPyramid(nullptr),
	m_bIsTransformed(false),
	m_bIsIdle(false),
	m_MovementQueue{}
{}

Coily::Coily(Coily const& other)
	:Enemy(other),
	m_pPyramid(other.m_pPyramid),
	m_bIsTransformed(other.IsTransformed()),
	m_bIsIdle(other.m_bIsIdle),
	m_CurrentlyInQueue(0)
{
}


void Coily::Initialize()
{
	m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Egg_Small.png");
	m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Egg_Big.png");
	Enemy::Initialize();
}

void Coily::Update()
{
	if (!m_bIsTransformed)
	{
		Enemy::Update();
		if (m_pCurrentQube->IsLastRow())
		{
			Transform();
		}
	}
	else if (!m_bIsIdle)
	{
		Enemy::Update();
	}
}

void Coily::Move(ConnectionDirection direction)
{
	if (direction != ConnectionDirection::null)
	{
		if (m_bIsTransformed)
		{
			SetDirectionTextures(direction);
		}
		m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pJumpText);
		Enemy::Move(direction);
	}
	else
	{
		if(m_pCurrentQube->IsSideColumn())
		{
			JumpToDeath(direction);
		}
		else
		{
			SetIsIdle(true);
		}
	}
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

void Coily::SetIsIdle(bool isIdle)
{
	m_bIsIdle = isIdle;
	if (!isIdle)
	{
		InitMovementQueue();
	}
}

void Coily::Die()
{
	Enemy::Die();
	m_pSubject->Notify(this, (int)EnemyEvents::CoilyDies);
}

void Coily::Transform()
{
	//m_pGameObject->GetComponent<RendererComponent>()->
		//SetTexture(empire::ResourceManager::GetInstance().GetTexture("Coily_FaceRight_Tall.png"));
	m_bIsTransformed = true;
	InitMovementQueue();
}

void Coily::InitMovementQueue()
{
	FindQBert();
}

ConnectionDirection Coily::ChooseDirection()
{
	if (!m_bIsTransformed)
	{
		return Enemy::ChooseDirection();
	}
	
	if (m_CurrentlyInQueue == 0)
	{
		FindQBert();
	}
	
	auto dirToReturn = m_MovementQueue[MOVEMENT_QUEUE_SIZE - m_CurrentlyInQueue];
	m_CurrentlyInQueue--;
	
	return  dirToReturn;
}

void Coily::FindQBert()
{
	if (m_pPyramid == nullptr)
	{
		Debugger::GetInstance().LogError("Variable m_pPyramid of Coily is undefined");
	}
	std::thread t1([this]
		{
			bool result{};

			do
			{
				result = m_pPyramid->FindNextQubeToQbert(m_pCurrentQube, m_MovementQueue, MOVEMENT_QUEUE_SIZE);
			} while (!result);
			m_CurrentlyInQueue = MOVEMENT_QUEUE_SIZE;
		});
	t1.detach();
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