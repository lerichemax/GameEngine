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
	writer.WriteString("txt", m_Text);

	Color Color{ m_TextColor.r, m_TextColor.g , m_TextColor.b, m_TextColor.a };

	writer.WriteObject("color", &Color);
	writer.WriteObject("font", m_pFont);
	m_pFont->Serialize(writer);

	Component::Serialize(writer);
}

void TextRendererComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadString("txt", m_Text);
	auto colorObject = reader->ReadObject("color");
	Color Color{ 0,0,0,0 };
	Color.Deserialize(colorObject.get());
	m_TextColor = SDL_Color{ Color.R, Color.G, Color.B, Color.A };

	auto pFontreader = reader->ReadObject("font");

	std::string fontName;
	pFontreader->ReadString("fontName", fontName);

	int size;
	pFontreader->ReadInt("fontSize", size);

	m_pFont = ResourceManager::Get().GetFont(fontName, size);

	m_NeedsUpdate = true;

	Component::Deserialize(reader, context);
}