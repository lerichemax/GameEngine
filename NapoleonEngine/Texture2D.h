#pragma once
struct SDL_Texture;
namespace empire
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D
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

	private:
		SDL_Texture* m_pTexture;
		float m_Width;
		float m_Height;
	};
}
