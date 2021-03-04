#include "MiniginPCH.h"
#include "EarnPointsCommand.h"
#include "QBert.h"

using namespace dae;

EarnPointsCommand::EarnPointsCommand(KeyActionState state, QBert* pPlayer)
	: Command{ state },
	m_pPlayerCharacter{ pPlayer }
{
}

bool EarnPointsCommand::Execute()
{
	m_pPlayerCharacter->EarnPoints();
	return true;
}