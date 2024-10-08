#include "PCH.h"
#include "Enemy.h"

#include "GameObject.h"

Enemy::Enemy(int pointsForKill, CharacterType type)
	:Characters(nullptr, type),
	POINTS_FOR_KILL(pointsForKill)
{}

Enemy::Enemy(Enemy const& other)
	:Characters(other),
	POINTS_FOR_KILL(other.POINTS_FOR_KILL)
{}

void Enemy::DoMove(ConnectionDirection direction)
{
	SetDirectionTextures(direction);
	
	//if (m_pCurrentQube->HasConnection(direction))
	//{
	//	/*auto* const pNextQube = m_pCurrentQube->GetConnection(direction);
	//	if (pNextQube->HasCharacter())
	//	{
	//		if (pNextQube->GetCharacter()->GetType() != CharacterType::player)
	//		{
	//			return;
	//		}
	//	}
	//	
	//	m_pCurrentQube->CharacterJumpOut();
	//	JumpToQube(pNextQube);*/
	//}
	//else
	//{
	//	JumpToDeath(direction);
	//}
}

void Enemy::DoDie()
{
	//if (m_pCurrentQube)
	//{
	//	m_pCurrentQube->CharacterJumpOut();
	//}
	//GetEntity()->Destroy();
}