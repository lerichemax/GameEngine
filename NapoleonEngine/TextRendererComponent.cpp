#include "PCH.h"
#include "TextRendererComponent.h"
#include "Renderer.h"

#include <SDL.h>
#include <SDL_ttf.h>

using namespace empire;

TextRendererComponent::TextRendererComponent(std::string const& text, Font* const pFont)
	:RendererComponent{},
	m_Text{text},
	m_pFont{pFont},
	m_NeedsUpdate{true},
	m_TextColor{255,255,255}
{
}

TextRendererComponent::~TextRendererComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void TextRendererComponent::Init(SceneObject* pParent)
{
	Component::Init(pParent);
	Update();
}

void TextRendererComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_TextColor);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto const texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		delete m_pTexture;
		m_pTexture = new Texture2D{ texture };
	}
}

void TextRendererComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void TextRendererComponent::SetTextColor(Uint8 r, Uint8 g, Uint8 b)
{
	m_TextColor = SDL_Color{ r,g,b };
}