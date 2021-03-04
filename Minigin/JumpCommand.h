#pragma once
#include "Command.h"

class JumpCommand final : public dae::Command
{
public:
	JumpCommand(KeyActionState state = KeyActionState::pressed) : Command(state) {}
	JumpCommand(JumpCommand const& other) = delete;
	JumpCommand(JumpCommand&& other) = delete;
	JumpCommand& operator=(JumpCommand const& rhs) = delete;
	JumpCommand& operator=(JumpCommand&& rhs) = delete;
	~JumpCommand() override = default;

	bool Execute() override;
};