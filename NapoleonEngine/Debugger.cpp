#include "PCH.h"
#include "Debugger.h"
#include <iostream>

using namespace empire;

void Debugger::Log(std::string const& message) const
{
	std::cout << message << std::endl;
}

void Debugger::LogError(std::string const& message) const
{
	std::cerr << message << std::endl;
}

void Debugger::DrawDebugLine(glm::vec2 const& startPoint, glm::vec2 const& endPoint) const
{
	glLineWidth(1.f);
	glBegin(GL_LINES);
	{
		glVertex2f(startPoint.x, startPoint.y);
		glVertex2f(endPoint.x, endPoint.y);
	}
	glEnd();
}

void Debugger::DrawDebugPoint(glm::vec2 const& pos, float thickness) const
{
	glPointSize(thickness);
	glBegin(GL_POINTS);
	{
		glVertex2f(pos.x, pos.y);
	}
	glEnd();
}

void Debugger::DrawDebugCircle(glm::vec2 const& center, float radius) const
{
	float angle{ float(M_PI / radius) };
	
	glLineWidth(1.f);
	glBegin(GL_LINE_LOOP);
	{
		for (float i = 0.0; i < float(2 * M_PI); i += angle)
		{
			glVertex2f(center.x + radius * float(cos(i)), center.y + radius * float(sin(i)));
		}
	}
	glEnd();
}