#include "PCH.h"
#include "Enemy.h"
#include "Qube.h"

#include "GameObject.h"
#include "QBert.h"

Enemy::Enemy(int pointsForKill, CharacterType type)
	:Character(nullptr, type),
	POINTS_FOR_KILL(pointsForKill)
{}

void Enemy::Move(ConnectionDirection direction)
{
	SetDirectionTextures(direction);
	
	if (m_pCurrentQube->HasConnection(direction))
	{
		auto pNextQube = m_pCurrentQube->GetConnection(direction);
		if (pNextQube->HasCharacter())
		{
			if (pNextQube->GetCharacter()->GetType() != CharacterType::player)
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