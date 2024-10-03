#pragma once
#include <SDL_opengl.h>

#include "Renderer.h"
#include "Singleton.h"
#include "Shapes.h"
#include <glm/glm.hpp>

using namespace geo;
using namespace glm;

class Debugger final : public Singleton<Debugger>
{
public:
	~Debugger();
	void Log(std::string const& message) const;
	void LogWarning(std::string const& message) const;
	void LogError(std::string const& message) const;
		
	void DrawDebugLine(vec2 const& startPoint, vec2 const& endPoint, Color const& Color);
	void DrawDebugPoint(vec2 const& Pos, unsigned int thickness, Color const& Color);
	void DrawDebugCircle(vec2 const& center, unsigned int radius, Color const& Color);
	void DrawDebugRectangle(vec2 const& Pos, unsigned int witdth, unsigned int Height, Color const& Color);
	void DrawDebugShape(geo::Shape* const pShape);
	
private:
	friend class Singleton<Debugger>;
	friend void Renderer::Render();
		
	Debugger();

	HANDLE m_ConsoleHandle;

	std::vector<geo::Shape*> m_Shapes;
	std::vector<geo::Shape*> m_OwnedShapes;

	void Render();
};
