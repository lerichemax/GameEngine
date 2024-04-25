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
		
	void DrawDebugLine(vec2 const& startPoint, vec2 const& endPoint, Color const& color);
	void DrawDebugPoint(vec2 const& pos, unsigned int thickness, Color const& color);
	void DrawDebugCircle(vec2 const& center, unsigned int radius, Color const& color);
	void DrawDebugRectangle(vec2 const& pos, unsigned int witdth, unsigned int height, Color const& color);
	
private:
	friend class Singleton<Debugger>;
	friend void Renderer::Render();
		
	Debugger();

	HANDLE m_ConsoleHandle;
		
	std::vector<Line> m_DebugLines;
	std::vector<geo::Rectangle> m_DebugRectangles;
	std::vector<geo::Circle> m_DebugCircles;
	std::vector<Point> m_DebugPoints;

	void Render();
};
