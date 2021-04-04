#pragma once
#include "Command.h"
#include "Qube.h"

class GameController;
class QBert;
class MoveCommand final : public empire::Command
{
public:
	MoveCommand(ConnectionDirection dir, QBert* pQbert, GameController* pController, KeyActionState state = KeyActionState::pressed);
	MoveCommand(MoveCommand const& other) = delete;
	MoveCommand(MoveCommand&& other) = delete;
	MoveCommand& operator=(MoveCommand const& rhs) = delete;
	MoveCommand& operator=(MoveCommand&& rhs) = delete;
	~MoveCommand() override = default;

	bool Execute() override;

private:
	ConnectionDirection m_MoveDirection;
	QBert* m_pQbert;
	GameController* m_pGameController;
};
