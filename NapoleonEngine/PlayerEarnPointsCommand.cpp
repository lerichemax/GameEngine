#include "MiniginPCH.h"
#include "PlayerEarnPointsCommand.h"
#include "QBert.h"

using namespace dae;

PlayerEarnPointsCommand::PlayerEarnPointsCommand(KeyActionState state, QBert* pPlayer)
	: Command{ state },
	m_pPlayerCharacter{ pPlayer }
{
}

bool PlayerEarnPointsCommand::Execute()
{
	m_pPlayerCharacter->EarnPoints();
	return true;
}