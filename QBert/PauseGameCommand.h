#pragma once
#include "Command.h"

class PauseGameCommand final : public empire::Command
{
public:
	PauseGameCommand();
	PauseGameCommand* Clone() { return new PauseGameCommand(*this); }
	PauseGameCommand(PauseGameCommand&& other) = delete;
	PauseGameCommand& operator=(PauseGameCommand const& rhs) = delete;
	PauseGameCommand& operator=(PauseGameCommand&& rhs) = delete;
	~PauseGameCommand() = default;
	
	void Execute() override;

private:
	bool m_bIsPaused;

	PauseGameCommand(PauseGameCommand const& other);
};