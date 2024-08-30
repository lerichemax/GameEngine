#include "PCH.h"
#include "TextRendererComponent.h"
#include "ResourceManager.h"

#include <SDL.h>
#include <SDL_ttf.h>

int TextRendererComponent::Id_Increment = 0;
EventHandler<TextRendererComponent, int> TextRendererComponent::OnAnyDestroyed{};

TextRendererComponent::TextRendererComponent()
	:Component(true)
{
	m_TextId = Id_Increment++;
}

TextRendererComponent::TextRendererComponent(std::string const& text, Font* const font)
	:Component(true),
	m_Text{text},
	m_pFont{font}
{
	m_TextId = Id_Increment++;
}

TextRendererComponent::~TextRendererComponent()
{
	OnAnyDestroyed.Notify(m_TextId);
}

void TextRendererComponent::SetText(std::string const& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void TextRendererComponent::SetFont(Font* const pFont)
{
	m_pFont = pFont;
	m_NeedsUpdate = true;
}

void TextRendererComponent::SetTextColor(Uint8 r, Uint8 g, Uint8 b)
{
	m_TextColor = SDL_Color{ r,g,b };
	m_NeedsUpdate = true;
}

void TextRendererComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(TextRendererComponent)).hash_code()));
	writer.WriteInt("txtId", m_TextId);
	writer.WriteString("txt", m_Text);
	Color color{ m_TextColor.r, m_TextColor.g , m_TextColor.b, m_TextColor.a };
	writer.StartObject("color");
	color.Serialize(writer);
	writer.EndObject();
	m_pFont->Serialize(writer);

	Component::Serialize(writer);
}

void TextRendererComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadInt("txtId", m_TextId);
	reader->ReadString("txt", m_Text);
	auto colorObject = reader->ReadObject("color");
	Color color{ 0,0,0,0 };
	color.Deserialize(colorObject.get());
	m_TextColor = SDL_Color{ color.R, color.G, color.B, color.A };

	std::string fontName;
	reader->ReadString("fontName", fontName);

	int size;
	reader->ReadInt("fontSize", size);

	m_pFont = ResourceManager::GetInstance().GetFont(fontName, size);

	m_NeedsUpdate = true;

	Component::Deserialize(reader, context);
}