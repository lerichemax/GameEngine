#include "PCH.h"
#include "Debugger.h"
#include <iostream>
#include "Shapes.h"

Debugger::Debugger()
	: Singleton(),
	m_ConsoleHandle(GetStdHandle(STD_OUTPUT_HANDLE)),
	m_DebugLines(),
	m_DebugRectangles(),
	m_DebugCircles(),
	m_DebugPoints()
{
	SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

Debugger::~Debugger()
{
	SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Debugger::Log(std::string const& message) const
{
	SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	std::cout << message << std::endl;
}

void Debugger::LogWarning(std::string const& message) const
{
	SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	std::cout << "WARNING : "<< message << std::endl;
}


void Debugger::LogError(std::string const& message) const
{
	MessageBox(0, message.c_str(), "Error!", 0);
	NapoleonEngine::Quit();
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
		m_DebugRectangles.push_back(geo::Rectangle(pos, thickness, thickness, color));
	}
}

void Debugger::DrawDebugCircle(glm::vec2 const& center, unsigned int radius , Color const& col)
{
	m_DebugCircles.push_back(Circle(center, radius, col));
}

void Debugger::DrawDebugRectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, Color const& color)
{
	m_DebugRectangles.push_back(geo::Rectangle{ pos, width, height, color });
}

void Debugger::Render()
{
	for (Line const& line : m_DebugLines)
	{
		Renderer::GetInstance().RenderShape(line);
	}

	for (geo::Rectangle const& rect : m_DebugRectangles)
	{
		Renderer::GetInstance().RenderShape(rect);
	}

	for (Circle const& circle : m_DebugCircles)
	{
		Renderer::GetInstance().RenderShape(circle);
	}

	for (Point const& point : m_DebugPoints)
	{
		Renderer::GetInstance().RenderShape(point);
	}

	m_DebugLines.clear();
	m_DebugRectangles.clear();
	m_DebugCircles.clear();
	m_DebugPoints.clear();
}