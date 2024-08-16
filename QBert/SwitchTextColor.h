#pragma once
#include <memory>

#include "Command.h"

#include "Shapes.h"


struct TextRendererComponent;
class SwitchTextColor final : public Command
{
public:
	SwitchTextColor() = default;
	SwitchTextColor(Color targetColor);

	SwitchTextColor(SwitchTextColor const& other) = delete;
	SwitchTextColor(SwitchTextColor&& other) = delete;
	SwitchTextColor& operator=(SwitchTextColor const& rhs) = delete;
	SwitchTextColor& operator=(SwitchTextColor&& rhs) = delete;
	~SwitchTextColor() = default;

	void Execute(GameObject* const gObject) override;

	void Serialize(StreamWriter& writer) const;
	void Deserialize(JsonReader const* reader, SerializationMap& context);

private:
	Color m_TargetColor{};
};
