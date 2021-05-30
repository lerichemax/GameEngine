#pragma once
#include "Command.h"

class SwitchScene final : public empire::Command
{
public:
	SwitchScene(std::string pTargetScene);
	SwitchScene* Clone() { return new SwitchScene(*this); }
	SwitchScene(SwitchScene&& other) = delete;
	SwitchScene& operator=(SwitchScene const& rhs) = delete;
	SwitchScene& operator=(SwitchScene&& rhs) = delete;
	void Execute() override;

private:
	std::string m_TargetSceneName;

	SwitchScene(SwitchScene const& other);
};