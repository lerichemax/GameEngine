#pragma once
#include "Command.h"

class QBert;
class PlayerDieCommand final : public dae::Command
{
public:
	PlayerDieCommand(KeyActionState state, QBert* pPlayer);

	bool Execute() override;

private:
	QBert* m_pPlayerCharacter;
};