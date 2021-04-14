#pragma once
#include "Command.h"

class FartCommand final : public empire::Command
{
public:
	FartCommand(KeyActionState state = KeyActionState::pressed) : Command(state) {}
	FartCommand(FartCommand const& other) = delete;
	FartCommand(FartCommand&& other) = delete;
	FartCommand& operator=(FartCommand const& rhs) = delete;
	FartCommand& operator=(FartCommand&& rhs) = delete;
	~FartCommand() override = default;

	void Execute() override;
};
