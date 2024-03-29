#pragma once
#include "Singleton.h"
#include "Shapes.h"

struct SDL_Window;
struct SDL_Renderer;
namespace empire
{
	struct Shape;
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
		void RenderShape(Shape const& pShape) const;
		void SetBackgroundColor(Color color) { m_BackgroundColor = color; }
		void SetBackgroundColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { SetBackgroundColor(Color{ r,g,b,a }); }
		
		static SDL_Renderer* GetSDLRenderer() { return m_pRenderer; }
	
	private:
		friend class Singleton<Renderer>;
		
		static SDL_Renderer* m_pRenderer;
		Renderer();
		
		SDL_Window* m_pWindow{};
		
		Color m_BackgroundColor;

		int GetOpenGLDriverIndex();
	};
}

