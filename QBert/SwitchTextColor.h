#pragma once
#include <memory>

#include "Command.h"

#include "Shapes.h"


class SwitchTextColor final : public Command
{
public:
	SwitchTextColor();
	SwitchTextColor(Color const& targetColor);
	SwitchTextColor(SwitchTextColor const& other);
	SwitchTextColor(SwitchTextColor&& other);
	SwitchTextColor& operator=(SwitchTextColor const& other);
	SwitchTextColor& operator=(SwitchTextColor&& other);

	~SwitchTextColor();

	void Execute(GameObject const& gObject) override;

private:
	PROPERTY(Color*, m_pTargetColor);
};

SERIALIZE_CLASS(SwitchTextColor, IContextSerializable)