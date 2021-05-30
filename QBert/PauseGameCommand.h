#pragma once
#include "Command.h"

class PauseGameCommand final : public empire::Command
{
public:
	PauseGameCommand(KeyActionState keyState);
	void Execute() override;

private:
	bool m_bIsPaused;
	
};