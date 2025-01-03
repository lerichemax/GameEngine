#pragma once
#include "Serializer.h"

struct SDL_Texture;
class Texture2D final
{
	friend class ResourceManager;

public:
	Texture2D(const Texture2D&) = delete;
	Texture2D(Texture2D&&) = delete;
	Texture2D& operator= (const Texture2D&) = delete;
	Texture2D& operator= (const Texture2D&&) = delete;
	~Texture2D();

	SDL_Texture* GetSDLTexture();
	float GetWidth() const { return m_Width; }
	float GetHeight() const { return m_Height; }

private:
	SDL_Texture* m_pTexture;

	PROPERTY(std::string, m_FilePath);

	float m_Width;
	float m_Height;

	explicit Texture2D(SDL_Texture* texture, std::string const& filePath);
	explicit Texture2D(SDL_Texture* texture); // use for font textures, they don't need a file path

	void InitTexture();
	void GetTexture();
};

SERIALIZE_CLASS(Texture2D);
