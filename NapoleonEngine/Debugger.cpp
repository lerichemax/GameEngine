#include "PCH.h"
#include "Debugger.h"
#include <iostream>
#include "Shapes.h"

using namespace empire;

Debugger::Debugger()
	: Singleton()
{ }

void Debugger::Log(std::string const& message) const
{
	std::cout << message << std::endl;
}

void Debugger::LogError(std::string const& message) const
{
	MessageBox(NULL, message.c_str(), "Error!", MB_OK);
}

void Debugger::DrawDebugLine(glm::vec2 const& startPos, glm::vec2 const& endPos, Color const& col)
{
	m_DebugLines.push_back(Line(startPos, endPos, col));
}

void Debugger::DrawDebugPoint(glm::vec2 const& pos, unsigned int thickness , Color const& color)
{
	if (thickness == 1 )
	{
		m_DebugPoints.push_back(Point(pos, color));
	}
	else
	{
		m_DebugRectangles.push_back(Rectangle(pos, thickness, thickness, color));
	}
}

void Debugger::DrawDebugCircle(glm::vec2 const& center, unsigned int radius , Color const& col)
{
	m_DebugCircles.push_back(Circle(center, radius, col));
}

void Debugger::DrawDebugRectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, Color const& color)
{
	m_DebugRectangles.push_back(Rectangle{ pos, width, height, color });
}

void Debugger::Render(SDL_Renderer* pRenderer)
{
	for (Line const& line : m_DebugLines)
	{
		line.Draw(pRenderer);
	}

	for (Rectangle const& rect : m_DebugRectangles)
	{
		rect.Draw(pRenderer);
	}

	for (Circle const& circle : m_DebugCircles)
	{
		circle.Draw(pRenderer);
	}

	for (Point const& point : m_DebugPoints)
	{
		point.Draw(pRenderer);
	}

	m_DebugLines.clear();
	m_DebugRectangles.clear();
	m_DebugCircles.clear();
	m_DebugPoints.clear();
}