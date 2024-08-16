#include "PCH.h"
#include "TextRendererComponent.h"
#include "ResourceManager.h"

#include <SDL.h>
#include <SDL_ttf.h>

TextRendererComponent::TextRendererComponent(std::string const& text, std::shared_ptr<Font> font)
	:ECS_Component(true),
	m_Text{text},
	m_pFont{font}
{

}

void TextRendererComponent::SetTextColor(Uint8 r, Uint8 g, Uint8 b)
{
	m_TextColor = SDL_Color{ r,g,b };
}

void TextRendererComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(TextRendererComponent).name());
	writer.WriteString("txt", m_Text);
	Color color{ m_TextColor.r, m_TextColor.g , m_TextColor.b, m_TextColor.a };
	writer.StartObject("color");
	color.Serialize(writer);
	writer.EndObject();
	m_pFont->Serialize(writer);

	ECS_Component::Serialize(writer);
}

void TextRendererComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadString("txt", m_Text);
	auto colorObject = reader->ReadObject("color");
	Color color{ 0,0,0,0 };
	color.Deserialize(reader);
	m_TextColor = SDL_Color{ color.R, color.G, color.B, color.A };

	std::string fontName;
	reader->ReadString("fontName", fontName);

	int size;
	reader->ReadInt("fontSize", size);

	m_pFont = ResourceManager::GetInstance().GetFont(fontName, size);

	ECS_Component::Deserialize(reader, context);
}