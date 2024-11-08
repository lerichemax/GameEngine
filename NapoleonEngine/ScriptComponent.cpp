#include "PCH.h"
#include "ScriptComponent.h"

void ScriptComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString(std::string{ "type" }, typeid(ScriptComponent).name());
	writer.WriteString("script", ScriptFile);

	Component::Serialize(writer);
}

void ScriptComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadString("script", ScriptFile);

	Component::Deserialize(reader, context);
}