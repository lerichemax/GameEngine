#pragma once
#include "Command.h"

class QuitCommand final : public empire::Command
{
public:
	QuitCommand(KeyActionState state = KeyActionState::pressed) : Command(state) {}
	QuitCommand(QuitCommand const& other) = delete;
	QuitCommand(QuitCommand&& other) = delete;
	QuitCommand& operator=(QuitCommand const& rhs) = delete;
	QuitCommand& operator=(QuitCommand&& rhs) = delete;
	~QuitCommand() override = default;

	void Execute() override;
};
