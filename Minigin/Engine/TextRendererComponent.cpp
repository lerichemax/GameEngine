#include "MiniginPCH.h"
#include "TextRendererComponent.h"
#include "Renderer.h"

#include <SDL.h>
#include <SDL_ttf.h>

using namespace dae;

TextRendererComponent::TextRendererComponent(std::string const& text, Font* const pFont)
	:RendererComponent{},
	m_Text{text},
	m_pFont{pFont},
	m_NeedsUpdate{true}
{
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
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
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
		m_pTexture = new Texture2D{ texture };
	}
}

void TextRendererComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}