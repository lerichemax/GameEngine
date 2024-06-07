#pragma once
#include "Serializer.h"

struct SDL_Texture;
/**
	* Simple RAII wrapper for an SDL_Texture
	*/
class Texture2D final : public ISerializable
{
public:
		
	explicit Texture2D(SDL_Texture* texture);
	Texture2D(const Texture2D&) = delete;
	Texture2D(Texture2D&&) = delete;
	Texture2D& operator= (const Texture2D&) = delete;
	Texture2D& operator= (const Texture2D&&) = delete;
	~Texture2D();

	SDL_Texture* GetSDLTexture() const;
	float GetWidth() const { return m_Width; }
	float GetHeight() const { return m_Height; }

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader) override;

private:
	SDL_Texture* m_pTexture;
	std::string m_FilePath;
	float m_Width;
	float m_Height;
};
