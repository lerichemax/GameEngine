#pragma once
#include "Command.h"


class GameObject;
class GameManagerBehaviour;
class PauseGameCommand final : public Command
{
public:
	PauseGameCommand() = default;
	PauseGameCommand(std::shared_ptr<GameManagerBehaviour> pGameManager);

	PauseGameCommand(PauseGameCommand const& other) = delete;
	PauseGameCommand(PauseGameCommand&& other) = delete;
	PauseGameCommand& operator=(PauseGameCommand const& rhs) = delete;
	PauseGameCommand& operator=(PauseGameCommand&& rhs) = delete;
	~PauseGameCommand() = default;
	
	void Execute(GameObject* const gObject) override;

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;

private:
	std::shared_ptr<GameManagerBehaviour> m_pGameManager;
};