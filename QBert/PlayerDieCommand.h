#pragma once
#include "Command.h"

class QBert;
class PlayerDieCommand final : public dae::Command
{
public:
	PlayerDieCommand();
	
	bool Execute() override;

private:
	QBert* m_pPlayerCharacter;
};