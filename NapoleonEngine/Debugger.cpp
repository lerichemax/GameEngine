#include "PCH.h"
#include "Debugger.h"
#include <iostream>
#include "Renderer.h"
#include "DebugShapes.h"

using namespace empire;

Debugger::~Debugger()
{
	for (auto pShape : m_DebugShapes)
	{
		delete pShape;
	}
	m_DebugShapes.clear();
}

Debugger::Debugger()
	: Singleton()
{ }

void Debugger::Log(std::string const& message) const
{
	std::cout << message << std::endl;
}

void Debugger::LogError(std::string const& message) const
{
	std::cerr << message << std::endl;
}

void Debugger::AddDebugLine(glm::vec2 const& startPos, glm::vec2 const& endPos, Color const& col)
{
	m_DebugShapes.push_back(new Line(startPos, endPos, col));
}

void Debugger::AddDebugPoint(glm::vec2 const& pos, unsigned int thickness , Color const& color)
{
	if (thickness == 1 )
	{
		m_DebugShapes.push_back(new Point(pos, color));
	}
	else
	{
		m_DebugShapes.push_back(new Rectangle(pos, thickness, thickness, color));
	}

	
}

void Debugger::AddDebugCircle(glm::vec2 const& center, unsigned int radius , Color const& col)
{
	m_DebugShapes.push_back(new Circle(center, radius, col));
}

void Debugger::Render(SDL_Renderer* pRenderer)
{
	for (auto shape : m_DebugShapes)
	{
		shape->Draw(pRenderer);
	}
	
	//m_DebugShapes.clear();
}