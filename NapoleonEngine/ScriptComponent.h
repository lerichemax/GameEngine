#pragma once
#include "Component.h"

struct ScriptComponent : public Component
{
	std::string ScriptFile;

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;
};