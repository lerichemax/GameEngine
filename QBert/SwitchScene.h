#pragma once
#include "Command.h"

class SwitchScene final : public Command
{
public:
	SwitchScene() = default;
	SwitchScene(std::string pTargetScene);

	SwitchScene(SwitchScene const& other) = delete;
	SwitchScene(SwitchScene&& other) = delete;
	SwitchScene& operator=(SwitchScene const& rhs) = delete;
	SwitchScene& operator=(SwitchScene&& rhs) = delete;
	void Execute(GameObject* const gObject) override;

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

private:
	std::string m_TargetSceneName;
};