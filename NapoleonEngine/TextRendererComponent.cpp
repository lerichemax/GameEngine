#include "PCH.h"
#include "TextRendererComponent.h"
#include "ResourceManager.h"

#include <SDL.h>
#include <SDL_ttf.h>

int TextRendererComponent::Id_Increment = 0;
EventHandler<TextRendererComponent, int> TextRendererComponent::OnAnyDestroyed{};

TextRendererComponent::TextRendererComponent()
	:Component(true),
	m_pFont{nullptr},
	m_pTextColor{ new Color{ 255,255,255,255 } }
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
	SAFE_DELETE(m_pTextColor);
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
	SAFE_DELETE(m_pTextColor);
	m_pTextColor = new Color{ r,g,b };
	m_NeedsUpdate = true;
}

SDL_Color TextRendererComponent::GetSDLColor() const
{
	return SDL_Color{ (*m_pTextColor).R, (*m_pTextColor).G, (*m_pTextColor).B, (*m_pTextColor).A };
}

void TextRendererComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("txt", m_Text);

	writer.WriteObject("color", m_pTextColor);
	writer.WriteObject("font", m_pFont);

	Component::Serialize(writer);
}

void TextRendererComponent::Deserialize(JsonReader* const reader, SerializationMap& context)
{
	reader->ReadString("txt", m_Text);
	auto colorObject = reader->ReadObject("color");
	m_pTextColor->Deserialize(colorObject.get());

	auto pFontreader = reader->ReadObject("font");

	std::string fontName;
	pFontreader->ReadString("fontName", fontName);

	int size;
	pFontreader->ReadInt("fontSize", size);

	m_pFont = ResourceManager::Get().GetFont(fontName, size);

	m_NeedsUpdate = true;

	Component::Deserialize(reader, context);
}