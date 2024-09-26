#include "PCH.h"
#include "JumpComponent.h"

void JumpComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(JumpComponent)).hash_code()));

	writer.StartObject("direction");
	{
		writer.WriteDouble("x", Direction.x);
		writer.WriteDouble("y", Direction.y);
	}
	writer.EndObject();

	Component::Serialize(writer);
}

void JumpComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	std::unique_ptr<JsonReader> pDirectionreader = reader->ReadObject("direction");
	pDirectionreader->ReadDouble("x", Direction.x);
	pDirectionreader->ReadDouble("y", Direction.y);

	Component::Deserialize(reader, context);
}

void JumpComponent::Reset()
{
	bJumpDown = false;
	bIsJumping = false;
	bIsFalling = false;
}
