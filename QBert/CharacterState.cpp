#include "PCH.h"
#include "CharacterState.h"

CharacterState::CharacterState(Character* pChar, Jumper* pJumper, CharacterStateType type)
	:m_pJumperComp(pJumper),
	m_pCharacter(pChar),
	m_Type(type)
{
}