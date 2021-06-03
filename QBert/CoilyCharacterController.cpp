#include "PCH.h"
#include "CoilyCharacterController.h"

#include <thread>

#include "Pyramid.h"
#include "Coily.h"
#include "Qube.h"

#include "Timer.h"

CoilyCharacterController::CoilyCharacterController() : EnemyCharacterController()
{
	
}

CoilyCharacterController::CoilyCharacterController(CoilyCharacterController const& other)
	:EnemyCharacterController(other),
	m_pPyramid(other.m_pPyramid),
	m_MovementQueue{},
	m_pCoilyCharacter(other.m_pCoilyCharacter),
	m_bIsIdle(other.m_bIsIdle),
	m_CurrentlyInQueue(other.m_CurrentlyInQueue)
	
{
	for (unsigned int i{}; i< MOVEMENT_QUEUE_SIZE; i++)
	{
		m_MovementQueue[i] = other.m_MovementQueue[i];
	}
}

void CoilyCharacterController::Initialize()
{
	EnemyCharacterController::Initialize();
	m_pCoilyCharacter = static_cast<Coily*>(m_pEnemyCharacter);
}

void CoilyCharacterController::Update()
{
	if (!m_pCoilyCharacter->IsTransformed())
	{
		EnemyCharacterController::Update();
		if (m_pCoilyCharacter->GetCurrentQube()->IsLastRow() && m_pCoilyCharacter->GetState() == State::onQube)
		{
			m_pCoilyCharacter->Transform(true);
			FindQBert();
		}
	}
	else if (!m_bIsIdle)
	{
		if (m_pEnemyCharacter->GetState() == State::onQube)
		{
			
			if (m_MoveTimer < MOVE_MAX_TIME)
			{
				m_MoveTimer += empire::Timer::GetInstance().GetDeltaTime();
				return;
			}

			if (m_CurrentlyInQueue != 0)
			{
				m_pEnemyCharacter->Move(ChooseDirection());
				m_MoveTimer = 0;
			}
		}
	}
}

void CoilyCharacterController::SetIdle(bool isIdle)
{
	m_bIsIdle = isIdle;
	m_MoveTimer = 0;
	if (!isIdle)
	{
		FindQBert();
	}
}

void CoilyCharacterController::Move(ConnectionDirection dir)
{
	if (dir != ConnectionDirection::null)
	{
		m_pCoilyCharacter->Move(dir);
	}
	else
	{
		if (m_pCoilyCharacter->GetCurrentQube()->IsSideColumn())
		{
			m_pCoilyCharacter->FallSound();
			m_pCoilyCharacter->JumpToDeath(dir);
		}
		else
		{
			SetIdle(true);
		}
	}
}

ConnectionDirection CoilyCharacterController::ChooseDirection()
{
	if (!m_pCoilyCharacter->IsTransformed())
	{
		return EnemyCharacterController::ChooseDirection();
	}

	
	auto dirToReturn = m_MovementQueue[MOVEMENT_QUEUE_SIZE - m_CurrentlyInQueue];
	m_CurrentlyInQueue--;
	

	return  dirToReturn;
}

void CoilyCharacterController::FindQBert()
{
	if (m_pPyramid == nullptr)
	{
		Debugger::GetInstance().LogError("Variable m_pPyramid of CoilyCharacterController is undefined");
	}
	std::thread t1([this]
		{
			bool result{};

			result = m_pPyramid->FindNextQubeToQbert(m_pCoilyCharacter->GetCurrentQube(), m_MovementQueue, MOVEMENT_QUEUE_SIZE);

			if (result)
			{
				m_CurrentlyInQueue = MOVEMENT_QUEUE_SIZE;
				m_bIsIdle = false;
			}
		});
	t1.join();
}