#pragma once
#include "Command.h"

class QBert;
class PlayerEarnPointsCommand final : public dae::Command
{
public:
	PlayerEarnPointsCommand(KeyActionState state, QBert* pPlayer);

	bool Execute() override;

private:
	QBert* m_pPlayerCharacter;
};
