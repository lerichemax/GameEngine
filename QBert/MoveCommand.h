#pragma once
#include "Command.h"
#include "Qube.h"

class Pyramid;
class QBert;
class MoveCommand final : public empire::Command
{
public:
	MoveCommand(ConnectionDirection dir, QBert* pQbert, Pyramid* pController, KeyActionState state = KeyActionState::pressed);
	MoveCommand(MoveCommand const& other) = delete;
	MoveCommand(MoveCommand&& other) = delete;
	MoveCommand& operator=(MoveCommand const& rhs) = delete;
	MoveCommand& operator=(MoveCommand&& rhs) = delete;
	~MoveCommand() override = default;

	void Execute() override;

private:
	ConnectionDirection m_MoveDirection;
	QBert* m_pQbert;
	Pyramid* m_pGameController;
};
