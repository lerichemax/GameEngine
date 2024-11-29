#pragma once
#include "Component.h"

struct ScriptComponent : public Component
{
	PROPERTY(std::string, ScriptFile);

	void Serialize(StreamWriter& writer) const;
	void Deserialize(JsonReader* const reader, SerializationMap& context);
};

SERIALIZE_CLASS(ScriptComponent)