#include "PCH.h"
#include "Texture2D.h"
#include <SDL.h>

empire::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* empire::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

empire::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_Texture = texture;
}
