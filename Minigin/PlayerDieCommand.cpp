#include "MiniginPCH.h"
#include "PlayerDieCommand.h"
#include "QBert.h"

using namespace dae;

PlayerDieCommand::PlayerDieCommand(KeyActionState state, QBert* pPlayer)
	: Command{ state },
	m_pPlayerCharacter{ pPlayer }
{
}

bool PlayerDieCommand::Execute()
{
	m_pPlayerCharacter->Die();
	return true;
}