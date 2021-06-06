#include "PCH.h"
#include "Texture2D.h"
#include "Renderer.h"

#include <SDL.h>

empire::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

SDL_Texture* empire::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

empire::Texture2D::Texture2D(SDL_Texture* texture)
	:m_pTexture(texture),
	m_Width(),
	m_Height()
{
	int width{};
	int height{};
	SDL_QueryTexture(m_pTexture, nullptr, nullptr, &width, &height);
	m_Width = static_cast<float>(width);
	m_Height = static_cast<float>(height);
}