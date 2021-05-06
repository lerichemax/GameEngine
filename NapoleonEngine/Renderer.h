#pragma once
#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;
namespace empire
{

	class RendererComponent;
	class Texture2D;
	class TransformComponent;
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		
		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, TransformComponent const& pTransform) const;

		
		static SDL_Renderer* GetSDLRenderer() { return m_pRenderer; }
	
	private:
		
		static SDL_Renderer* m_pRenderer;
		SDL_Window* m_pWindow{};
		
		bool m_ShowDemo{};

		int GetOpenGLDriverIndex();
	};
}

