#include "PCH.h"
#include "Shapes.h"

using namespace geo;

void Color::Serialize(StreamWriter& writer) const
{
	writer.WriteInt("r", R);
	writer.WriteInt("g", G);
	writer.WriteInt("b", B);
	writer.WriteInt("a", A);
}

void Color::Deserialize(JsonReader const* reader)
{
	if (reader == nullptr || !reader->IsValid())
	{
		return;
	}

	int r, g, b, a;

	reader->ReadInt("r", r);
	reader->ReadInt("g", g);
	reader->ReadInt("b", b);
	reader->ReadInt("a", a);

	R = static_cast<uint8_t>(r);
	G = static_cast<uint8_t>(g);
	B = static_cast<uint8_t>(b);
	A = static_cast<uint8_t>(a);
}

Point::Point(glm::vec2 const& pos, Color const& col)
	:Shape(col),
	pos{ pos }
{
}

void Point::Draw(SDL_Renderer* pRenderer) const
{
	SDL_SetRenderDrawColor(pRenderer, color.R, color.G, color.B, 255);
	SDL_RenderDrawPoint(pRenderer, static_cast<int>(pos.x), static_cast<int>(pos.y));
}



Rectangle::Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, Color const& col, bool filled)
	:Shape(col),
	pos{ pos },
	width{ width },
	height{ height },
	isFilled(filled)
{
}

Rectangle::Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, bool filled)
	:Shape(),
	pos{pos},
	width{width},
	height{height},
	isFilled(filled)
{
	
}


Rectangle::Rectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool filled)
	:Shape(),
	pos{x,y},
	width(width),
	height(height),
	isFilled(filled)
{
}

void Rectangle::Draw(SDL_Renderer* pRenderer) const
{
	if (!isFilled)
	{
		SDL_SetRenderDrawColor(pRenderer, color.R, color.G, color.B, 255);
		SDL_Rect rect{ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(width),
			static_cast<int>(height) };
		SDL_RenderDrawRect(pRenderer, &rect);
	}
	else
	{
		Fill(pRenderer);
	}
}

void Rectangle::Fill(SDL_Renderer* pRenderer) const
{
	SDL_SetRenderDrawColor(pRenderer, color.R, color.G, color.B, color.A);
	SDL_Rect rect{ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(width),
		static_cast<int>(height) };
	SDL_RenderFillRect(pRenderer, &rect);
}

/// <summary>
/// Copyright DAE prog 2 framework utils::IsOverlapping
/// </summary>
bool geo::AreOverlapping(geo::Rectangle const& rec1, geo::Rectangle const& rec2)
{
	if ((rec1.pos.x + rec1.width) < rec2.pos.x || (rec2.pos.x + rec2.width) < rec1.pos.x)
	{
		return false;
	}

	if (rec1.pos.y > (rec2.pos.y + rec2.height) || rec2.pos.y > (rec1.pos.y + rec1.height))
	{
		return false;
	}

	return true;
}

/// <summary>
/// Copyright DAE prog 2 framework utils::IsOverlapping
/// </summary>
bool geo::AreOverlapping(geo::Rectangle const& rec1, Circle const& c2)
{
	if (geo::IsPointInRec(c2.center, rec1))
	{
		return true;
	}
	if (geo::DistPointLine(c2.center, rec1.pos, glm::vec2{ rec1.pos.x, rec1.pos.y + rec1.height }) <= c2.radius)
	{
		return true;
	}
	if (geo::DistPointLine(c2.center, rec1.pos, glm::vec2{ rec1.pos.x + rec1.width, rec1.pos.y }) <= c2.radius)
	{
		return true;
	}
	if (geo::DistPointLine(c2.center, glm::vec2{ rec1.pos.x + rec1.width, rec1.pos.y + rec1.height }, 
		glm::vec2{ rec1.pos.x, rec1.pos.y + rec1.height }) <= c2.radius)
	{
		return true;
	}
	if (geo::DistPointLine(c2.center, glm::vec2{ rec1.pos.x + rec1.width, rec1.pos.y + rec1.height },
		glm::vec2{ rec1.pos.x + rec1.width, rec1.pos.y }) <= c2.radius)
	{
		return true;
	}
	
	return false;
}

/// <summary>
/// Copyright DAE prog 2 framework utils::DistPointLineSegment
/// </summary>
bool geo::AreOverlapping(Circle const& c1, Circle const& c2)
{
	// squared distance between centers
	float const xDistance{ c1.center.x - c2.center.x };
	float const yDistance{ c1.center.y - c2.center.y };
	float const squaredDistance{ xDistance * xDistance + yDistance * yDistance };

	float const squaredTouchingDistance { static_cast<float>(c1.radius + c2.radius) * (c1.radius + c2.radius) };
	return (squaredDistance < squaredTouchingDistance);
}


/// <summary>
/// Copyright DAE prog 2 framework utils::DistPointLineSegment
/// </summary>
unsigned int geo::DistPointLine(glm::vec2 const& p, glm::vec2 const& a, glm::vec2 const& b)
{
	//vectors
	glm::vec2 ab{ b - a};
	glm::vec2 ap{ p - a };
	glm::vec2 const abNorm{ glm::normalize(ab) };
	float const distToA{ glm::dot(abNorm,ap) };


	if (distToA < 0)
	{
		return ap.length();
	}

	auto const distAB{ ab.length() };
	if (static_cast<int>(distToA) > distAB)
	{
		return glm::vec2{ p - b }.length();
	}
	
	glm::vec2 const intersection{ glm::dot(abNorm,ap) * abNorm +  a  };
	return (p - intersection).length();
}

bool geo::IsPointInRec(glm::vec2 const& p, geo::Rectangle const& rec)
{
	return (p.x >= rec.pos.x &&
		p.x <= rec.pos.x + rec.width &&
		p.y >= rec.pos.y &&
		p.y <= rec.pos.y + rec.height);
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
	SDL_RenderDrawLine(pRenderer, static_cast<int>(startPos.x), static_cast<int>(startPos.y),
		static_cast<int>(endPos.x), static_cast<int>(endPos.y));
}

Circle::Circle(glm::vec2 const& center, unsigned int radius, Color const& col)
	:Shape(col),
	center(center),
	radius(radius)
{}

Circle::Circle(glm::vec2 const& center, unsigned int radius)
	:Shape(),
	center(center),
	radius(radius)
{
}

void Circle::Draw(SDL_Renderer* pRenderer) const
{
	float const dAngle{ static_cast<float>(M_PI)/radius };

	SDL_SetRenderDrawColor(pRenderer, color.R, color.G, color.B, 255);
	

	for (float angle = 0.0; angle < static_cast<float>(2 * M_PI); angle += dAngle)
	{
		SDL_RenderDrawPoint(pRenderer, static_cast<int>(center.x + radius * cos(angle)), 
			static_cast<int>(center.y + radius * sin(angle)));
	}
}
