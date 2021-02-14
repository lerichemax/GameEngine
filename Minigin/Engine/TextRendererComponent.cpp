#include "MiniginPCH.h"
#include "TextRendererComponent.h"
#include "Renderer.h"

#include <SDL.h>
#include <SDL_ttf.h>

dae::TextRendererComponent::TextRendererComponent(Font* const pFont)
	:RendererComponent{},
	m_pFont{pFont}
{
}

void dae::TextRendererComponent::Update(std::string const& text)
{
	const SDL_Color color = { 255,255,255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), color);
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