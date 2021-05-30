#pragma once
#include "Command.h"
#include "Qube.h"

class Pyramid;
class QBert;
class MoveCommand final : public empire::Command
{
public:
	MoveCommand(ConnectionDirection dir, Character* pChar);
	~MoveCommand() = default;

	void Execute() override;

private:
	ConnectionDirection m_MoveDirection;
	Character* m_pCharacter;
};
