#pragma once
#include <memory>

#include "Command.h"

#include "Shapes.h"


struct ECS_TextRendererComponent;
class SwitchTextColor final : public Command
{
public:
	SwitchTextColor(Color targetColor, std::shared_ptr<ECS_TextRendererComponent> pText);
	SwitchTextColor* Clone() override { return new SwitchTextColor(*this); }
	SwitchTextColor(SwitchTextColor&& other) = delete;
	SwitchTextColor& operator=(SwitchTextColor const& rhs) = delete;
	SwitchTextColor& operator=(SwitchTextColor&& rhs) = delete;
	~SwitchTextColor() = default;

	void Execute() override;

private:
	Color m_TargetColor;
	std::shared_ptr<ECS_TextRendererComponent> m_pText;

	SwitchTextColor(SwitchTextColor const& other);
};
