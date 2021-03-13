#include "PCH.h"
#include "EarnPointsCommand.h"

#include "GameObject.h"
#include "QBert.h"

using namespace empire;

EarnPointsCommand::EarnPointsCommand(KeyActionState state, GameObject* const pPlayer)
	: Command{ state },
	m_pPlayerCharacter{ pPlayer }
{
}

bool EarnPointsCommand::Execute()
{
	m_pPlayerCharacter->GetComponent<QBert>().EarnPoints();
	return true;
}