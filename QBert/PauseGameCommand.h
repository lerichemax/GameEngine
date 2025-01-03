#pragma once
#include "Command.h"


class GameObject;
class GameManagerSystem;
class PauseGameCommand final : public Command
{
public:
	PauseGameCommand() = default;
	PauseGameCommand(GameManagerSystem* const pGameManager);

	~PauseGameCommand() = default;
	
	void Execute(GameObject const& gObject) override;

private:
	GameManagerSystem* m_pGameManager; // we do not own the ressource, so shallow copy is ok.
};

SERIALIZE_CLASS(PauseGameCommand, IContextSerializable)