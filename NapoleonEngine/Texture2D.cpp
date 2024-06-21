#include "PCH.h"
#include "Texture2D.h"
#include "Renderer.h"

#include <SDL.h>

Texture2D::Texture2D(SDL_Texture* texture)
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

Texture2D::Texture2D(SDL_Texture* texture, std::string const& filePath)
	:m_pTexture(texture),
	m_FilePath(filePath),
	m_Width(),
	m_Height()
{
	int width{};
	int height{};
	SDL_QueryTexture(m_pTexture, nullptr, nullptr, &width, &height);
	m_Width = static_cast<float>(width);
	m_Height = static_cast<float>(height);
}

Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

SDL_Texture* Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

void Texture2D::Serialize(StreamWriter& writer) const
{
	writer.WriteString("filepath", m_FilePath);
}

void Texture2D::Deserialize(JsonReader const* reader)
{
	reader->ReadString("filepath", m_FilePath);
}