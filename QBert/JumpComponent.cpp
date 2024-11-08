#include "PCH.h"
#include "JumpComponent.h"

void JumpComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString(std::string{ "type" }, typeid(JumpComponent).name());

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
