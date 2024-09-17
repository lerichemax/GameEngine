#include "PCH.h"
#include "ScriptComponent.h"

void ScriptComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(ScriptComponent)).hash_code()));
	writer.WriteString("script", ScriptFile);

	Component::Serialize(writer);
}

void ScriptComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadString("script", ScriptFile);

	Component::Deserialize(reader, context);
}