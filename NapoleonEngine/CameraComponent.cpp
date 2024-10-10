#include "PCH.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent()
	:m_Width(Renderer::Get().GetWindowWidth()),
	m_Height(Renderer::Get().GetWindowHeight())
{
}

void CameraComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CameraComponent).name());
	writer.WriteInt("width", m_Width);
	writer.WriteInt("height", m_Height);
}

void CameraComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	int Width, Height;
	reader->ReadInt("width", Width);
	reader->ReadInt("height", Height);
	m_Width = Width;
	m_Height = Height;
}