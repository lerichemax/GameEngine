#include "PCH.h"
#include "Enemy.h"
#include "Qube.h"

#include "GameObject.h"
#include "QBert.h"

Enemy::Enemy(int pointsForKill)
	:Character(nullptr, Type::enemy),
	POINTS_FOR_KILL(pointsForKill)
{}

void Enemy::Initialize()
{
	Character::Initialize();
}

void Enemy::MoveToCurrentQube()
{
	Character::MoveToCurrentQube();
}

void Enemy::JumpToQube(Qube* pTargetQube)
{
	Character::JumpToQube(pTargetQube);
}

void Enemy::Update()
{
	
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