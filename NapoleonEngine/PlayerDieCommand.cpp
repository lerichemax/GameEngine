#include "MiniginPCH.h"
#include "PlayerDieCommand.h"
#include "GameObject.h"
#include "QBert.h"

using namespace dae;

PlayerDieCommand::PlayerDieCommand(KeyActionState state, GameObject* pPlayer)
	: Command{ state },
	m_pPlayerCharacter{ pPlayer }
{
}

bool PlayerDieCommand::Execute()
{
	m_pPlayerCharacter->GetComponent<QBert>().Die();
	return true;
}