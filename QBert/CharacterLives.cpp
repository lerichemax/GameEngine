#include "PCH.h"
#include "CharacterLives.h"

CharacterLives::CharacterLives(int maxLives)
	:MAX_LIVES(maxLives),
	m_NbrLives(maxLives),
	m_bIsGameOver(false)
{
	
}

void CharacterLives::Die()
{
	m_NbrLives--;
	if (m_NbrLives <= 0)
	{
		m_bIsGameOver = true;
	}
}

void CharacterLives::Reset()
{
	m_NbrLives = MAX_LIVES;
	m_bIsGameOver = false;
}

ECS_CharacterLives::ECS_CharacterLives(int maxLives)
	:ECS_Component(true),
	MAX_LIVES(maxLives),
	m_NbrLives(maxLives)
{

}

void ECS_CharacterLives::Die()
{
	m_NbrLives--;
}

void ECS_CharacterLives::Reset()
{
	m_NbrLives = MAX_LIVES;
}