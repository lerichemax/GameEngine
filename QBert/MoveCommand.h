#pragma once
#include "Command.h"
#include "Qube.h"

class Pyramid;
class QBert;
class MoveCommand final : public empire::Command
{
public:
	MoveCommand(ConnectionDirection dir, Character* pChar);
	MoveCommand* Clone() { return new MoveCommand(*this); }
	MoveCommand(MoveCommand&& other) = delete;
	MoveCommand& operator=(MoveCommand const& rhs) = delete;
	MoveCommand& operator=(MoveCommand&& rhs) = delete;
	~MoveCommand() = default;

	void Execute() override;

private:
	ConnectionDirection m_MoveDirection;
	Character* m_pCharacter;

	MoveCommand(MoveCommand const& other);
};
