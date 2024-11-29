#include "PCH.h"
#include "TextRendererComponent.h"
#include "ResourceManager.h"

#include <SDL.h>
#include <SDL_ttf.h>

int TextRendererComponent::Id_Increment = 0;
EventHandler<TextRendererComponent, int> TextRendererComponent::OnAnyDestroyed{};

TextRendererComponent::TextRendererComponent()
	:Component(true),
	m_pFont{ new Font{} },
	m_pTextColor{ new Color{ 255,255,255,255 } }
{
	m_TextId = Id_Increment++;
}

TextRendererComponent::TextRendererComponent(std::string const& text, std::string const& fullPath, int size)
	:Component(true),
	m_Text{text},
	m_pFont{ ResourceManager::Get().GetFont(fullPath, size) }
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
