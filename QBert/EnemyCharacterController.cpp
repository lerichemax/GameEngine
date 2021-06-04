#include "PCH.h"
#include "EnemyCharacterController.h"

#include "Character.h"
#include "Enemy.h"
#include "Qube.h"

#include "GameObject.h"
#include "Timer.h"

EnemyCharacterController::EnemyCharacterController()
	:Component(),
	m_pEnemyCharacter(nullptr)
{
	
}

EnemyCharacterController::EnemyCharacterController(EnemyCharacterController const& other)
	:m_pEnemyCharacter(other.m_pEnemyCharacter),
	m_MoveTimer(other.m_MoveTimer)
{
	
}

void EnemyCharacterController::Initialize()
{
	m_pEnemyCharacter = m_pGameObject->GetComponent<Enemy>();
}

ConnectionDirection EnemyCharacterController::ChooseDirection()
{
	int random = rand() % 2 + 1; //down left or down right

	auto dir = (ConnectionDirection)random;

	if (!m_pEnemyCharacter->GetCurrentQube()->HasConnection(dir))
	{
		if (dir == ConnectionDirection::downLeft)
		{
			dir = ConnectionDirection::downRight;
		}
		else if (dir == ConnectionDirection::downRight)
		{
			dir = ConnectionDirection::downLeft;
		}
	}

	return dir;
}