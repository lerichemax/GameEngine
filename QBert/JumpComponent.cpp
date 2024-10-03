#include "PCH.h"
#include "JumpComponent.h"

void JumpComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(JumpComponent)).hash_code()));

	writer.WriteVector("direction", Direction);

	Component::Serialize(writer);
}

void JumpComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadVector("direction", Direction);

	Component::Deserialize(reader, context);
}

void JumpComponent::Reset()
{
	bJumpDown = false;
	bIsJumping = false;
	bIsFalling = false;
}
