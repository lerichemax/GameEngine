#include "PCH.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include <SDL.h>

Texture2D::Texture2D()
	:m_pTexture(nullptr),
	m_Width(),
	m_Height()
{
}

Texture2D::Texture2D(SDL_Texture* texture)
	:m_pTexture(texture),
	m_Width(),
	m_Height()
{
	InitTexture();
}

Texture2D::Texture2D(SDL_Texture* texture, std::string const& filePath)
	:m_pTexture(texture),
	m_FilePath(filePath),
	m_Width(),
	m_Height()
{
	InitTexture();
}
void Texture2D::InitTexture()
{
	int Width{};
	int Height{};
	SDL_QueryTexture(m_pTexture, nullptr, nullptr, &Width, &Height);
	m_Width = static_cast<float>(Width);
	m_Height = static_cast<float>(Height);
}

Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

SDL_Texture* Texture2D::GetSDLTexture()
{
	if (!IS_VALID(m_pTexture))
	{
		GetTexture();
	}
	return m_pTexture;
}

void Texture2D::GetTexture()
{
	if (!m_FilePath.empty())
	{
		m_pTexture = ResourceManager::Get().GetSDLTexture(m_FilePath);
	}
}