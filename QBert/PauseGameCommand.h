#pragma once
#include "Command.h"


class GameObject;
class GameManagerSystem;
class PauseGameCommand final : public Command
{
public:
	PauseGameCommand() = default;
	PauseGameCommand(GameManagerSystem* const pGameManager);

	PauseGameCommand(PauseGameCommand const& other) = delete;
	PauseGameCommand(PauseGameCommand&& other) = delete;
	PauseGameCommand& operator=(PauseGameCommand const& rhs) = delete;
	PauseGameCommand& operator=(PauseGameCommand&& rhs) = delete;
	~PauseGameCommand() = default;
	
	void Execute(GameObject const& gObject) override;

private:
	GameManagerSystem* m_pGameManager;
};

SERIALIZE_CLASS(PauseGameCommand, IContextSerializable)