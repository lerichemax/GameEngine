#include "PCH.h"
#include "MovementComponent.h"
#include "QubeComponent.h"

MovementComponent::MovementComponent()
	:CurrentDirection{ConnectionDirection::null},
	CurrentQube(NULL_ENTITY)
{

}

void MovementComponent::SetTextureIdleNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft)
{
	TextureIdleDownRight = downRight;
	TextureIdleDownLeft = downLeft;
	TextureIdleUpRight = upRight;
	TextureIdleUpLeft = upLeft;
}

void MovementComponent::SetTextureJumpNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft)
{
	TextureJumpDownRight = downRight;
	TextureJumpDownLeft = downLeft;
	TextureJumpUpRight = upRight;
	TextureJumpUpLeft = upLeft;
}

void MovementComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(MovementComponent)).hash_code()));

	writer.WriteString("textIdleDownRight", TextureIdleDownRight);
	writer.WriteString("textIdleDownLeft", TextureIdleDownLeft);
	writer.WriteString("textIdleUpRight", TextureIdleUpRight);
	writer.WriteString("textIdleUpLeft", TextureIdleUpLeft);

	writer.WriteString("textJumpDownRight", TextureJumpDownRight);
	writer.WriteString("textJumpDownLeft", TextureJumpDownLeft);
	writer.WriteString("textJumpUpRight", TextureJumpUpRight);
	writer.WriteString("textJumpUpLeft", TextureJumpUpLeft);

	Component::Serialize(writer);
}

void MovementComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	Component::Deserialize(reader, context);

	reader->ReadString("textIdleDownRight", TextureIdleDownRight);
	reader->ReadString("textIdleDownLeft", TextureIdleDownLeft);
	reader->ReadString("textIdleUpRight", TextureIdleUpRight);
	reader->ReadString("textIdleUpLeft", TextureIdleUpLeft);

	reader->ReadString("textJumpDownRight", TextureJumpDownRight);
	reader->ReadString("textJumpDownLeft", TextureJumpDownLeft);
	reader->ReadString("textJumpUpRight", TextureJumpUpRight);
	reader->ReadString("textJumpUpLeft", TextureJumpUpLeft);
}
