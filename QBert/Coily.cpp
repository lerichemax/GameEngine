#include "PCH.h"
#include "Coily.h"
#include "Qube.h"

#include "GameObject.h"
#include "QBert.h"
#include "ResourceManager.h"
#include "RendererComponent.h"

Coily::Coily(Pyramid* pPyramid, Qube* pQube)
	: Enemy(pQube, 500),
	m_pPyramid(pPyramid),
	m_bIsTransformed(false)
{}

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
		Enemy::Move(direction);
	}
	else
	{
		if(m_pCurrentQube->IsSideColumn())
		{
			Enemy::Move(direction);
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
	m_pGameObject->GetComponent<RendererComponent>()->
		SetTexture(empire::ResourceManager::GetInstance().GetTexture("Coily_FaceRight_Tall.png"));
	m_bIsTransformed = true;
	InitMovementQueue();
}

void Coily::InitMovementQueue()
{
	m_pPyramid->FindNextQubeToQbert(m_pCurrentQube, m_MovementQueue, MOVEMENT_QUEUE_SIZE);
	m_CurrentlyInQueue = MOVEMENT_QUEUE_SIZE;
}

ConnectionDirection Coily::ChooseDirection()
{
	if (!m_bIsTransformed)
	{
		return Enemy::ChooseDirection();
	}
	
	if (m_CurrentlyInQueue == 0)
	{
		m_pPyramid->FindNextQubeToQbert(m_pCurrentQube, m_MovementQueue, MOVEMENT_QUEUE_SIZE);
		m_CurrentlyInQueue = MOVEMENT_QUEUE_SIZE;
	}
	
	auto dirToReturn = m_MovementQueue[MOVEMENT_QUEUE_SIZE - m_CurrentlyInQueue];
	m_CurrentlyInQueue--;
	
	return  dirToReturn;
}