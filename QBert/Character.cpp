#include "PCH.h"
#include "Character.h"
#include "GameObject.h"

Character::Character() 
	: m_pCurrentQube(),
	m_pSubject(new Subject{})
{
	
}

void Character::SetCurrentQube(Qube* pTargetQube)
{
	if (pTargetQube == nullptr)
	{
		return;
	}

	m_pCurrentQube = pTargetQube;
	m_pCurrentQube->CharacterJumpIn(this);
	m_pGameObject->GetTransform()->Translate(pTargetQube->GetCharacterPos());
}