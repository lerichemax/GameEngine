#pragma once
#include "Command.h"

class PauseGameCommand final : public empire::Command
{
public:
	PauseGameCommand();
	void Execute() override;

private:
	bool m_bIsPaused;
	
};