#pragma once
#include "Command.h"

class FireCommand final : public empire::Command
{
public:
	FireCommand(KeyActionState state = KeyActionState::pressed) : Command(state) {}
	FireCommand(FireCommand const& other) = delete;
	FireCommand(FireCommand&& other) = delete;
	FireCommand& operator=(FireCommand const& rhs) = delete;
	FireCommand& operator=(FireCommand&& rhs) = delete;
	~FireCommand() override = default;

	bool Execute() override;
};
