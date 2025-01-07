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

	void RenderTexture(Texture2D& texture, float x, float y) const;
	void RenderTexture(Texture2D& texture, float x, float y, float Width, float Height) const;
	void RenderTexture(Texture2D& texture, TransformComponent& pTransform) const;
	void RenderShape(geo::Shape const& pShape, glm::vec2 const& loc) const;
		
	static SDL_Renderer* GetSDLRenderer() { return m_pRenderer; }

	int GetWindowWidth() const { return m_WindowWidth; }
	int GetWindowHeight() const { return m_WindowHeight; }
	
private:
	friend class Singleton<Renderer>;
		
	static SDL_Renderer* m_pRenderer;
	Renderer();
		
	SDL_Window* m_pWindow{};

	int m_WindowWidth{};
	int m_WindowHeight{};

	int GetOpenGLDriverIndex();

	void CreateSDLWindow(unsigned int Width, unsigned int Height, std::string const& name);
};
