#include "PCH.h"
#include "CharacterLives.h"

CharacterLives::CharacterLives()
	:Component(true)
{

}

void CharacterLives::Init(int nbrLives)
{
	MaxLives = nbrLives;
	NbrLives = MaxLives;
}

void CharacterLives::Die()
{
	NbrLives--;
	bIsDead = true;
}

void CharacterLives::Reset()
{
	NbrLives = MaxLives;
	bIsDead = false;
}
