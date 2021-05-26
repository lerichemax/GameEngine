#include "PCH.h"
#include "Shapes.h"

using namespace empire;

Point::Point(glm::vec2 const& pos, Color const& col)
	:Shape(col),
	pos{ pos }
{
}

void Point::Draw(SDL_Renderer* pRenderer) const
{
	SDL_SetRenderDrawColor(pRenderer, color.R, color.G, color.B, 255);
	SDL_RenderDrawPoint(pRenderer, (int)pos.x, (int)pos.y);
}

Rectangle::Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, Color const& col)
	:Shape(col),
	pos{ pos },
	width{ width },
	height{ height }
{
}

Rectangle::Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height)
	:Shape(),
	pos{pos},
	width{width},
	height{height}
{
	
}


Rectangle::Rectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	:Shape(),
	pos{x,y},
	width(width),
	height(height)
{
}

void Rectangle::Draw(SDL_Renderer* pRenderer) const
{
	SDL_SetRenderDrawColor(pRenderer, color.R, color.G, color.B, 255);
	SDL_Rect rect{ (int)pos.x, (int)pos.y, (int)width, (int)height };
	SDL_RenderFillRect(pRenderer, &rect);
}

bool Rectangle::IsOverlapping(Rectangle const& rec2)
{
	if ((pos.x + width) < rec2.pos.x || (rec2.pos.x + rec2.width) < pos.x)
	{
		return false;
	}

	if (pos.y > (rec2.pos.y + rec2.height) || rec2.pos.y > (pos.y + height))
	{
		return false;
	}

	return true;
}

bool Rectangle::IsOverlapping(Circle const& )
{
	return false;
}

Line::Line(glm::vec2 const& startPos, glm::vec2 const& endPos, Color const& col)
	:Shape(col),
	startPos(startPos),
	endPos(endPos)
{
}

void Line::Draw(SDL_Renderer* pRenderer) const
{
	SDL_SetRenderDrawColor(pRenderer, color.R, color.G, color.B, 255);
	SDL_RenderDrawLine(pRenderer, (int)startPos.x, (int)startPos.y, (int)endPos.x, (int)endPos.y);
}

Circle::Circle(glm::vec2 const& center, unsigned int radius, Color const& col)
	:Shape(col),
	center(center),
	radius(radius)
{}


void Circle::Draw(SDL_Renderer* pRenderer) const
{
	float dAngle{ (float)M_PI/radius };

	SDL_SetRenderDrawColor(pRenderer, color.R, color.G, color.B, 255);
	

	for (float angle = 0.0; angle < float(2 * M_PI); angle += dAngle)
	{
		SDL_RenderDrawPoint(pRenderer, int(center.x + radius * float(cos(angle))), int(center.y + radius * float(sin(angle))));
	}
}
