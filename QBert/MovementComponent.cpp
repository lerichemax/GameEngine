#include "PCH.h"
#include "MovementComponent.h"

void MovementComponent::SetTextureIdleNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft)
{
	m_TextureIdleDownRight = downRight;
	m_TextureIdleDownLeft = downLeft;
	m_TextureIdleUpRight = upRight;
	m_TextureIdleUpLeft = upLeft;
}

void MovementComponent::SetTextureJumpNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft)
{
	m_TextureJumpDownRight = downRight;
	m_TextureJumpDownLeft = downLeft;
	m_TextureJumpUpRight = upRight;
	m_TextureJumpUpLeft = upLeft;
}

void MovementComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(MovementComponent).name());

	writer.WriteString("textIdleDownRight", m_TextureIdleDownRight);
	writer.WriteString("textIdleDownLeft", m_TextureIdleDownLeft);
	writer.WriteString("textIdleUpRight", m_TextureIdleUpRight);
	writer.WriteString("textIdleUpLeft", m_TextureIdleUpLeft);

	writer.WriteString("textJumpDownRight", m_TextureJumpDownRight);
	writer.WriteString("textJumpDownLeft", m_TextureJumpDownLeft);
	writer.WriteString("textJumpUpRight", m_TextureJumpUpRight);
	writer.WriteString("textJumpUpLeft", m_TextureJumpUpLeft);

	Component::Serialize(writer);
}

void MovementComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	Component::Deserialize(reader, context);

	reader->ReadString("textIdleDownRight", m_TextureIdleDownRight);
	reader->ReadString("textIdleDownLeft", m_TextureIdleDownLeft);
	reader->ReadString("textIdleUpRight", m_TextureIdleUpRight);
	reader->ReadString("textIdleUpLeft", m_TextureIdleUpLeft);

	reader->ReadString("textJumpDownRight", m_TextureJumpDownRight);
	reader->ReadString("textJumpDownLeft", m_TextureJumpDownLeft);
	reader->ReadString("textJumpUpRight", m_TextureJumpUpRight);
	reader->ReadString("textJumpUpLeft", m_TextureJumpUpLeft);
}