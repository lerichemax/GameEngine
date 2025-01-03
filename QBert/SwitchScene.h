#pragma once
#include "Command.h"

class SwitchScene final : public Command
{
public:
	SwitchScene() = default;
	SwitchScene(std::string pTargetScene);

	void Execute(GameObject const& gObject) override;

private:
	PROPERTY(std::string, m_TargetSceneName);
};

SERIALIZE_CLASS(SwitchScene, IContextSerializable)