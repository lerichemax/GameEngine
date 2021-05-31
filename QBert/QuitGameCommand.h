#pragma once
#include "Command.h"

class QuitGameCommand final : public empire::Command
{
public:
	QuitGameCommand() = default;
	QuitGameCommand* Clone() override { return new QuitGameCommand(*this); }
	QuitGameCommand(QuitGameCommand&& other) = delete;
	QuitGameCommand& operator=(QuitGameCommand const& rhs) = delete;
	QuitGameCommand& operator=(QuitGameCommand&& rhs) = delete;
	void Execute() override;

private:

	QuitGameCommand(QuitGameCommand const& ){}
	
};