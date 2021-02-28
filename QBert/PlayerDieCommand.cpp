#include "MiniginPCH.h"
#include "PlayerDieCommand.h"
#include "QBert.h"

using namespace dae;

PlayerDieCommand::PlayerDieCommand()
	: Command{ KeyActionState::pressed } {}

bool PlayerDieCommand::Execute()
{
	m_pPlayerCharacter->Die();
	return true;
}