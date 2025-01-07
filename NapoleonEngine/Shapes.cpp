#include "PCH.h"
#include "Shapes.h"

using namespace geo;

Color& Color::operator=(Color&& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;

	return *this;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b)
	:R{ r },
	G{ g },
	B{ b },
	A{ 255 }
{
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	:R{ r },
	G{ g },
	B{ b },
	A{ a }
{
}

void Color::Print() {
	printf("R: %d G: %d B: %d A: %d \n", R, G, B, A);
}

//Shape

geo::Shape::Shape(ShapeType type, glm::vec2 const& pos, Color const& col)
	: Type{ type },
	Pos{ pos },
	pShapeColor{ new Color{ col } } 
{}

geo::Shape::~Shape() {
	delete pShapeColor;
};

bool geo::Shape::IsOverlapping(Shape* const pOther) const
{
	switch (pOther->Type)
	{
	case ShapeType::Circle:
		return IsOverlapping(static_cast<Circle*>(pOther));
	case ShapeType::Rectangle:
		return IsOverlapping(static_cast<Rectangle*>(pOther));
	case ShapeType::Other:
		return false;
	default:
		return false;
	}
}

//Point
Point::Point()
	:Shape{ ShapeType::Other }
{
}

Point::Point(glm::vec2 const& pos, Color const& col)
	:Shape(ShapeType::Other, pos, col)
{

}

void Point::Draw(SDL_Renderer* pRenderer, glm::vec2 const& loc) const
{
	SDL_SetRenderDrawColor(pRenderer, pShapeColor->R, pShapeColor->G, pShapeColor->B, 255);
	SDL_RenderDrawPoint(pRenderer, static_cast<int>(loc.x + Pos.x), static_cast<int>(loc.y + Pos.y));
}

Rectangle::Rectangle()
	:Shape{ShapeType::Rectangle}
{

}

Rectangle::Rectangle(glm::vec2 const& pos, int Width, int Height, Color const& col, bool filled)
	:Shape(ShapeType::Rectangle, pos, col),
	Width{ Width },
	Height{ Height },
	bIsFilled(filled)
{
}

Rectangle::Rectangle(glm::vec2 const& pos, int Width, int Height, bool filled)
	:Shape(ShapeType::Rectangle, pos),
	Width{Width},
	Height{Height},
	bIsFilled(filled)
{
}


Rectangle::Rectangle(int x, int y, int Width, int Height, bool filled)
	:Shape(ShapeType::Rectangle, {x, y}),
	Width(Width),
	Height(Height),
	bIsFilled(filled)
{

}

void Rectangle::Draw(SDL_Renderer* pRenderer, glm::vec2 const& loc) const
{
	if (!bIsFilled)
	{
		SDL_SetRenderDrawColor(pRenderer, pShapeColor->R, pShapeColor->G, pShapeColor->B, 255);
		SDL_Rect rect{ static_cast<int>(loc.x + Pos.x), static_cast<int>(loc.y + Pos.y), static_cast<int>(Width),
			static_cast<int>(Height) };
		SDL_RenderDrawRect(pRenderer, &rect);
	}
	else
	{
		Fill(pRenderer, loc);
	}
}

void Rectangle::Fill(SDL_Renderer* pRenderer, glm::vec2 const& loc) const
{
	SDL_SetRenderDrawColor(pRenderer, pShapeColor->R, pShapeColor->G, pShapeColor->B, pShapeColor->A);
	SDL_Rect rect{ static_cast<int>(loc.x + Pos.x), static_cast<int>(loc.y + Pos.y), static_cast<int>(Width),
		static_cast<int>(Height) };
	SDL_RenderFillRect(pRenderer, &rect);
}

bool Rectangle::IsOverlapping(Rectangle* const pRec) const
{
	return AreOverlapping(*this, *pRec);
}

bool Rectangle::IsOverlapping(Circle* const pCircle) const
{
	return AreOverlapping(*this, *pCircle);
}

bool geo::AreOverlapping(geo::Rectangle const& rec1, geo::Rectangle const& rec2)
{
	if ((rec1.Pos.x + rec1.Width) < rec2.Pos.x || (rec2.Pos.x + rec2.Width) < rec1.Pos.x)
	{
		return false;
	}

	if (rec1.Pos.y > (rec2.Pos.y + rec2.Height) || rec2.Pos.y > (rec1.Pos.y + rec1.Height))
	{
		return false;
	}

	return true;
}

bool geo::AreOverlapping(geo::Rectangle const& rec1, Circle const& c2)
{
	if (geo::IsPointInRec(c2.Pos, rec1))
	{
		return true;
	}
	if (geo::DistPointLine(c2.Pos, rec1.Pos, glm::vec2{ rec1.Pos.x, rec1.Pos.y + rec1.Height }) <= c2.Radius)
	{
		return true;
	}
	if (geo::DistPointLine(c2.Pos, rec1.Pos, glm::vec2{ rec1.Pos.x + rec1.Width, rec1.Pos.y }) <= c2.Radius)
	{
		return true;
	}
	if (geo::DistPointLine(c2.Pos, glm::vec2{ rec1.Pos.x + rec1.Width, rec1.Pos.y + rec1.Height },
		glm::vec2{ rec1.Pos.x, rec1.Pos.y + rec1.Height }) <= c2.Radius)
	{
		return true;
	}
	if (geo::DistPointLine(c2.Pos, glm::vec2{ rec1.Pos.x + rec1.Width, rec1.Pos.y + rec1.Height },
		glm::vec2{ rec1.Pos.x + rec1.Width, rec1.Pos.y }) <= c2.Radius)
	{
		return true;
	}
	
	return false;
}

bool geo::AreOverlapping(Circle const& c1, Circle const& c2)
{
	// squared distance between centers
	float const xDistance{ c1.Pos.x - c2.Pos.x };
	float const yDistance{ c1.Pos.y - c2.Pos.y };
	float const squaredDistance{ xDistance * xDistance + yDistance * yDistance };

	float const squaredTouchingDistance { static_cast<float>(c1.Radius + c2.Radius) * (c1.Radius + c2.Radius) };
	return (squaredDistance < squaredTouchingDistance);
}

int geo::DistPointLine(glm::vec2 const& p, glm::vec2 const& a, glm::vec2 const& b)
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
	return (p.x >= rec.Pos.x &&
		p.x <= rec.Pos.x + rec.Width &&
		p.y >= rec.Pos.y &&
		p.y <= rec.Pos.y + rec.Height);
}

//Line
Line::Line()
	:Shape(ShapeType::Other)
{
}

Line::Line(glm::vec2 const& startPos, glm::vec2 const& EndPos, Color const& col)
	:Shape(ShapeType::Other, startPos, col),
	EndPos(EndPos)
{
}

void Line::Draw(SDL_Renderer* pRenderer, glm::vec2 const& loc) const
{
	SDL_SetRenderDrawColor(pRenderer, pShapeColor->R, pShapeColor->G, pShapeColor->B, 255);
	SDL_RenderDrawLine(pRenderer, static_cast<int>(loc.x + Pos.x), static_cast<int>(loc.y + Pos.y),
		static_cast<int>(loc.x + EndPos.x), static_cast<int>(loc.y + EndPos.y));
}

// Circle
Circle::Circle()
	:Shape{ShapeType::Circle}
{
}
Circle::Circle(glm::vec2 const& center, int Radius, Color const& col)
	:Shape(ShapeType::Circle, center, col),
	Radius(Radius)
{
}

Circle::Circle(glm::vec2 const& center, int Radius)
	:Shape(ShapeType::Circle, center),
	Radius(Radius)
{

}

void Circle::Draw(SDL_Renderer* pRenderer, glm::vec2 const& loc) const
{
	if (!bIsFilled)
	{
		float const dAngle{ static_cast<float>(M_PI) / Radius };

		for (float angle = 0.0; angle < static_cast<float>(2 * M_PI); angle += dAngle)
		{
			SDL_RenderDrawPoint(pRenderer, static_cast<int>((loc.x + Pos.x) + Radius * cos(angle)),
				static_cast<int>((loc.y + Pos.y) + Radius * sin(angle)));
		}
	}
	else 
	{
		Fill(pRenderer, loc);
	}

}

void Circle::Fill(SDL_Renderer* pRenderer, glm::vec2 const& loc) const
{
	SDL_SetRenderDrawColor(pRenderer, pShapeColor->R, pShapeColor->G, pShapeColor->B, 255);

	for (int w = 0; w < Radius * 2; w++)
	{
		for (int h = 0; h < Radius * 2; h++)
		{
			int dx = Radius - w; // horizontal offset
			int dy = Radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (Radius * Radius))
			{
				SDL_RenderDrawPoint(pRenderer, static_cast<int>(loc.x + Pos.x) + dx, static_cast<int>(loc.y + Pos.y) + dy);
			}
		}
	}
}

bool Circle::IsOverlapping(Rectangle* const pRec) const
{
	return AreOverlapping(*pRec, *this);
}

bool Circle::IsOverlapping(Circle* const pCircle) const
{
	return AreOverlapping(*this, *pCircle);
}

