#include "PCH.h"
#include "Debugger.h"
#include <iostream>
#include "Shapes.h"

Debugger::Debugger()
	: Singleton(),
	m_ConsoleHandle(GetStdHandle(STD_OUTPUT_HANDLE)),
	m_Shapes()
{
	SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

Debugger::~Debugger()
{
	SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	for (geo::Shape* pShape : m_OwnedShapes)
	{
		SAFE_DELETE(pShape);
	}

	m_OwnedShapes.clear();
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

void Debugger::DrawDebugLine(glm::vec2 const& startPos, glm::vec2 const& EndPos, Color const& col)
{
	m_OwnedShapes.push_back(new Line(startPos, EndPos, col));
}

void Debugger::DrawDebugPoint(glm::vec2 const& Pos, unsigned int thickness , Color const& Color)
{
	if (thickness == 1 )
	{
		m_OwnedShapes.push_back(new Point(Pos, Color));
	}
	else
	{
		m_OwnedShapes.push_back(new geo::Rectangle(Pos, thickness, thickness, Color, true));
	}
}

void Debugger::DrawDebugCircle(glm::vec2 const& center, unsigned int radius , Color const& col)
{
	m_OwnedShapes.push_back(new Circle(center, radius, col));
}

void Debugger::DrawDebugRectangle(glm::vec2 const& Pos, unsigned int Width, unsigned int Height, Color const& Color)
{
	m_OwnedShapes.push_back(new geo::Rectangle{ Pos, Width, Height, Color });
}

void Debugger::DrawDebugShape(geo::Shape* const pShape)
{
	m_Shapes.push_back(pShape);
}

void Debugger::Render()
{
	for (geo::Shape* pShape : m_Shapes)
	{
		Renderer::Get().RenderShape(*pShape);
	}
	
	for (geo::Shape* pShape : m_OwnedShapes)
	{
		Renderer::Get().RenderShape(*pShape);
		SAFE_DELETE(pShape);
	}

	m_Shapes.clear();
	m_OwnedShapes.clear();
}