#pragma once
#include <memory>

#include "Command.h"

#include "Shapes.h"


class SwitchTextColor final : public Command
{
public:
	SwitchTextColor();
	SwitchTextColor(Color const& targetColor);

	SwitchTextColor(SwitchTextColor const& other) = delete;
	SwitchTextColor(SwitchTextColor&& other) = delete;
	SwitchTextColor& operator=(SwitchTextColor const& rhs) = delete;
	SwitchTextColor& operator=(SwitchTextColor&& rhs) = delete;
	~SwitchTextColor() = default;

	void Execute(GameObject const& gObject) override;

private:
	PROPERTY(Color*, m_pTargetColor);
};

SERIALIZE_CLASS(SwitchTextColor, IContextSerializable)