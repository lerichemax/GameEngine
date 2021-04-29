#include "PCH.h"
#include "Enemy.h"
#include "Qube.h"

#include "GameObject.h"
#include "Timer.h"
#include "QBert.h"

Enemy::Enemy(Qube* pQube, int pointsForKill)
	:Character(pQube, Type::enemy),
	POINTS_FOR_KILL(pointsForKill)
{}

void Enemy::Initialize()
{
	MoveToCurrentQube();
}

void Enemy::Update()
{
	if (m_State == State::onQube)
	{
		if (m_MoveTimer < MOVE_MAX_TIME)
	{
		m_MoveTimer += empire::Timer::GetInstance().GetDeltaTime();
		return;
	}
	
	Move(ChooseDirection());
	m_MoveTimer = 0;
	}
	
	Character::Update();
}

void Enemy::Move(ConnectionDirection direction)
{
	if (m_pCurrentQube->HasConnection(direction))
	{
		auto pNextQube = m_pCurrentQube->GetConnection(direction);
		if (pNextQube->HasCharacter())
		{
			if (pNextQube->GetCharacter()->GetType() == Type::enemy)
			{
				return;
			}
		}
		
		m_pCurrentQube->CharacterJumpOut();
		JumpToQube(pNextQube);
	}
	else
	{
		JumpToDeath(direction);
	}
}

void Enemy::Die()
{
	if (m_pCurrentQube)
	{
		m_pCurrentQube->CharacterJumpOut();
	}
	m_pGameObject->Destroy();
}

ConnectionDirection Enemy::ChooseDirection()
{
	int random = rand() % 2 + 1; //down left or down right

	auto dir = (ConnectionDirection)random ;

	if (!m_pCurrentQube->HasConnection(dir))
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