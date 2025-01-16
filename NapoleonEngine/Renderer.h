#pragma once
#include "Singleton.h"
#include "Shapes.h"

using namespace geo;

struct SDL_Window;
struct SDL_Renderer;
struct Shape;
class Texture2D;
class TransformComponent;
class Camera2D;

namespace ecs 
{
	class Registry;
}

struct SDLDestroyer
{
	void operator()(SDL_Window* pWindow) const;
	void operator()(SDL_Renderer* pRenderer) const;
};

class Renderer final
{
public:
	Renderer(unsigned int Width, unsigned int Height, std::string const& name);
	~Renderer();

	void Render(ecs::Registry* const pRegistry, Color const& backgroundColor);
	void Destroy();

	void RenderTexture(Texture2D& texture, float x, float y) const;
	void RenderTexture(Texture2D& texture, float x, float y, float Width, float Height) const;
	void RenderTexture(Texture2D& texture, glm::mat3 const& transform) const;
	void RenderShape(geo::Shape const& pShape, glm::vec2 const& loc) const;
		
	SDL_Renderer* const GetSDLRenderer() { return m_pRenderer.get(); }

	int GetWindowWidth() const { return m_WindowWidth; } // maybe remove
	int GetWindowHeight() const { return m_WindowHeight; } // maybe remove
	
private:
	friend class Singleton<Renderer>;

	Renderer();
		
	std::unique_ptr<SDL_Renderer, SDLDestroyer> m_pRenderer{};
	std::unique_ptr<SDL_Window, SDLDestroyer> m_pWindow{};

	int m_WindowWidth{};
	int m_WindowHeight{};

	int GetOpenGLDriverIndex();

	void CreateSDLWindow(unsigned int Width, unsigned int Height, std::string const& name);
};
