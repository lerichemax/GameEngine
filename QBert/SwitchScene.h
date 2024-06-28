#pragma once
#include "Command.h"

class SwitchScene final : public Command
{
public:
	SwitchScene() = default;
	SwitchScene(std::string pTargetScene);
	SwitchScene* Clone() override { return new SwitchScene(*this); }
	SwitchScene(SwitchScene&& other) = delete;
	SwitchScene& operator=(SwitchScene const& rhs) = delete;
	SwitchScene& operator=(SwitchScene&& rhs) = delete;
	void Execute() override;

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override {};


private:
	std::string m_TargetSceneName;

	SwitchScene(SwitchScene const& other);
};