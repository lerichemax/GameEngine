#pragma once
#include "Command.h"

class QBertScene;
class ReloadSceneCommand final : public Command
{
public:
	ReloadSceneCommand() = default;
	ReloadSceneCommand(std::string scenName);

	ReloadSceneCommand(ReloadSceneCommand const& other) = delete;
	ReloadSceneCommand(ReloadSceneCommand&& other) = delete;
	ReloadSceneCommand& operator=(ReloadSceneCommand const& rhs) = delete;
	ReloadSceneCommand& operator=(ReloadSceneCommand&& rhs) = delete;
	~ReloadSceneCommand() = default;

	void Execute(GameObject* const gObject) override;

	void Serialize(StreamWriter& writer) const override {};
	void Deserialize(JsonReader const* reader, SerializationMap& context) override {};

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override {};

private:
	std::string m_SceneName;
};