#pragma once
#include "Command.h"

#include "Shapes.h"


class TextRendererComponent;
class TextRendererComponent;
class Pyramid;
class QBert;
class SwitchTextColor final : public Command
{
public:
	SwitchTextColor(Color targetColor, TextRendererComponent* pText);
	SwitchTextColor* Clone() override { return nullptr; } //unclonable
	SwitchTextColor(SwitchTextColor&& other) = delete;
	SwitchTextColor& operator=(SwitchTextColor const& rhs) = delete;
	SwitchTextColor& operator=(SwitchTextColor&& rhs) = delete;
	~SwitchTextColor() = default;

	void Execute() override;

private:
	Color m_TargetColor;
	TextRendererComponent* m_pText;

	SwitchTextColor(SwitchTextColor const& other);
};
