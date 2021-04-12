#include "PCH.h"
#include "Enemy.h"
#include "GameObject.h"
#include "Timer.h"

Enemy::Enemy()
	:Character()
{}

void Enemy::Update()
{
	if (m_MoveTimer < MOVE_MAX_TIME)
	{
		m_MoveTimer += Timer::GetInstance().GetDeltaTime();
		return;
	}
	
	Move(ChooseDirection());
	m_MoveTimer = 0;
}

void Enemy::Move(ConnectionDirection direction)
{
	if (m_pCurrentQube->HasConnection(direction))
	{
		m_pCurrentQube->CharacterJumpOut();
		SetCurrentQube(m_pCurrentQube->GetConnection(direction));
		m_pCurrentQube->CharacterJumpIn(this);
	}
	else
	{
		Die();
	}
}

void Enemy::Die()
{
	m_pGameObject->Destroy();
}

ConnectionDirection Enemy::ChooseDirection() const
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