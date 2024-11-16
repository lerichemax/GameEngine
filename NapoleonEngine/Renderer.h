#pragma once
#include "Singleton.h"
#include "Shapes.h"

using namespace geo;

struct SDL_Window;
struct SDL_Renderer;
struct Shape;
class Texture2D;
class TransformComponent;

namespace ecs 
{
	class Registry;
}

class Renderer final
{
public:
	Renderer(unsigned int Width, unsigned int Height, std::string const& name);
	~Renderer();

	void Render(ecs::Registry* const pRegistry, Color const& backgroundColor);
	void Destroy();

	void RenderTexture(const Texture2D& texture, float x, float y) const;
	void RenderTexture(const Texture2D& texture, float x, float y, float Width, float Height) const;
	void RenderTexture(const Texture2D& texture, TransformComponent const& pTransform) const;
	void RenderShape(geo::Shape const& pShape) const;
		
	static SDL_Renderer* GetSDLRenderer() { return m_pRenderer; }

	unsigned int GetWindowWidth() const { return m_WindowWidth; }
	unsigned int GetWindowHeight() const { return m_WindowHeight; }
	
private:
	friend class Singleton<Renderer>;
		
	static SDL_Renderer* m_pRenderer;
	Renderer();
		
	SDL_Window* m_pWindow{};

	unsigned int m_WindowWidth{};
	unsigned int m_WindowHeight{};

	int GetOpenGLDriverIndex();

	void CreateSDLWindow(unsigned int Width, unsigned int Height, std::string const& name);
};
