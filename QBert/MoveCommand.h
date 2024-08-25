#pragma once
#include "Command.h"

class CharacterMovement;
enum class ConnectionDirection;
class MoveCommand final : public Command
{
public:
	MoveCommand() = default;
	MoveCommand(ConnectionDirection dir);

	MoveCommand(MoveCommand const& other) = delete;
	MoveCommand(MoveCommand&& other) = delete;
	MoveCommand& operator=(MoveCommand const& rhs) = delete;
	MoveCommand& operator=(MoveCommand&& rhs) = delete;
	~MoveCommand() = default;

	void Execute(GameObject* const gObject) override;

private:
	ConnectionDirection m_MoveDirection;
};
