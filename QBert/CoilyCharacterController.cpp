#include "PCH.h"
#include "CoilyCharacterController.h"

#include <thread>

#include "Coily.h"
#include "QBert.h"

#include "Timer.h"
#include "GameObject.h"

CoilyCharacterController::CoilyCharacterController()
	: EnemyCharacterController(),
	MovementQueue{},
	CurrentlyInQueue(0),
	m_pCoilyCharacter(nullptr),
	m_pPyramid(nullptr),
	m_bIsIdle(false)
{
	
}

CoilyCharacterController::CoilyCharacterController(CoilyCharacterController const& other)
	:EnemyCharacterController(other),
	MovementQueue{},
	CurrentlyInQueue(other.CurrentlyInQueue),
	m_pCoilyCharacter(other.m_pCoilyCharacter),
	m_pPyramid(other.m_pPyramid),
	m_bIsIdle(other.m_bIsIdle)
{
	for (unsigned int i{}; i< MOVEMENT_QUEUE_SIZE; i++)
	{
		MovementQueue[i] = other.MovementQueue[i];
	}
}

void CoilyCharacterController::Initialize()
{
	EnemyCharacterController::Initialize();
	m_pCoilyCharacter = dynamic_cast<Coily*>(m_pEnemyCharacter);
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
	//if (dir != ConnectionDirection::null)
	//{
	//	m_pCoilyCharacter->Move(dir);
	//}
	//else
	//{
	//	if (m_pCoilyCharacter->GetCurrentQube()->IsSideColumn())
	//	{
	//		//m_pGameObject->GetParent()->GetComponent<Pyramid>()->GetQBert()->EarnPoints(m_pCoilyCharacter->GetPointsForKill());
	//		m_pCoilyCharacter->FallSound();
	//		m_pCoilyCharacter->JumpToDeath(dir);
	//	}
	//	else
	//	{
	//		SetIdle(true);
	//	}
	//}
}

ConnectionDirection CoilyCharacterController::ChooseDirection()
{
	if (!m_pCoilyCharacter->IsTransformed())
	{
		return EnemyCharacterController::ChooseDirection();
	}

	
	auto const dirToReturn = MovementQueue[MOVEMENT_QUEUE_SIZE - CurrentlyInQueue];
	CurrentlyInQueue--;
	

	return  dirToReturn;
}

void CoilyCharacterController::FindQBert()
{
	if (m_pPyramid == nullptr)
	{
		Debugger::Get().LogError("Variable m_pPyramid of CoilyCharacterController is undefined");
	}
	std::thread t1([this]
		{
			/*bool result{};

			result = m_pPyramid->TryFindPathTo(m_pCoilyCharacter->GetCurrentQube(), m_MovementQueue, MOVEMENT_QUEUE_SIZE);

			if (result)
			{
				m_CurrentlyInQueue = MOVEMENT_QUEUE_SIZE;
				m_bIsIdle = false;
			}*/
		});
	t1.join();
}