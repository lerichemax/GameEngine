#include "PCH.h"
#include "PlayerDieCommand.h"
#include "QBert.h"
#include "GameObject.h"

using namespace empire;

PlayerDieCommand::PlayerDieCommand(KeyActionState state, GameObject* pPlayer)
	: Command{ state },
	m_pPlayerCharacter{ pPlayer }
{
}

bool PlayerDieCommand::Execute()
{
	m_pPlayerCharacter->GetComponent<QBert>()->Die();
	return true;
}