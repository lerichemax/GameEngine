#pragma once
#include "Singleton.h"
#include "Shapes.h"

using namespace geo;

struct SDL_Window;
struct SDL_Renderer;
struct Shape;
struct RendererComponent;
class Texture2D;
class TransformComponent;
class ECS_TransformComponent;
class Renderer final : public Singleton<Renderer>
{
public:
	~Renderer();

	void Init(unsigned int width, unsigned int height, std::string const& name);
	void Render();
	void Destroy();

	void RenderTexture(const Texture2D& texture, float x, float y) const;
	void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
	void RenderTexture(const Texture2D& texture, TransformComponent const& pTransform) const;
	void RenderTexture(const Texture2D& texture, ECS_TransformComponent const& pTransform) const;
	void RenderShape(geo::Shape const& pShape) const;
	void SetBackgroundColor(Color color) { m_BackgroundColor = color; }
	void SetBackgroundColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { SetBackgroundColor(Color{ r,g,b,a }); }
		
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
		
	Color m_BackgroundColor;

	int GetOpenGLDriverIndex();

	void CreateSDLWindow(unsigned int width, unsigned int height, std::string const& name);
};
