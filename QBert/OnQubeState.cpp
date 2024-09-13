#include "PCH.h"
#include "OnQubeState.h"

#include "Character.h"

#include "GameObject.h"

OnQubeState::OnQubeState(Characters* pChar, Jumper* pJumper)
	:CharacterState(pChar, pJumper, CharacterStateType::onQube)
{
}

void OnQubeState::Enter()
{
	m_pCharacter->SwitchToIdleTex();
}
