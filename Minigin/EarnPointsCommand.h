#pragma once
#include "Command.h"

class QBert;
class EarnPointsCommand final : public dae::Command
{
public:
	EarnPointsCommand(KeyActionState state, QBert* pPlayer);

	bool Execute() override;

private:
	QBert* m_pPlayerCharacter;
};
