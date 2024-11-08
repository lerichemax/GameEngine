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

//Shape
void geo::Shape::Serialize(StreamWriter& writer) const
{
	writer.WriteVector("position", Pos);
	writer.WriteObject("color", &ShapeColor);
}

void geo::Shape::Deserialize(JsonReader const* reader)
{
	auto colorObj = reader->ReadObject("color");

	ShapeColor.Deserialize(colorObj.get());
}

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
	Factory<geo::Shape>::Get().RegisterType<Point>([]() {
		return new Point{};
		});
}

void Point::Draw(SDL_Renderer* pRenderer) const
{
	SDL_SetRenderDrawColor(pRenderer, ShapeColor.R, ShapeColor.G, ShapeColor.B, 255);
	SDL_RenderDrawPoint(pRenderer, static_cast<int>(Pos.x), static_cast<int>(Pos.y));
}

void Point::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(Point).name());

	Shape::Serialize(writer);
}

void Point::Deserialize(JsonReader const* reader)
{
	auto posObject = reader->ReadObject("position");
	posObject->ReadDouble("x", Pos.x);
	posObject->ReadDouble("y", Pos.y);

	Shape::Deserialize(reader);
}

Rectangle::Rectangle()
	:Shape{ShapeType::Rectangle}
{

}

Rectangle::Rectangle(glm::vec2 const& pos, unsigned int Width, unsigned int Height, Color const& col, bool filled)
	:Shape(ShapeType::Rectangle, pos, col),
	Width{ Width },
	Height{ Height },
	bIsFilled(filled)
{
	Factory<geo::Shape>::Get().RegisterType<Rectangle>([]() {
		return new Rectangle{};
		});
}

Rectangle::Rectangle(glm::vec2 const& pos, unsigned int Width, unsigned int Height, bool filled)
	:Shape(ShapeType::Rectangle, pos),
	Width{Width},
	Height{Height},
	bIsFilled(filled)
{
	Factory<geo::Shape>::Get().RegisterType<Rectangle>([]() {
		return new Rectangle{};
		});
}


Rectangle::Rectangle(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height, bool filled)
	:Shape(ShapeType::Rectangle, {x, y}),
	Width(Width),
	Height(Height),
	bIsFilled(filled)
{
	Factory<geo::Shape>::Get().RegisterType<Rectangle>([]() {
		return new Rectangle{};
		});
}

void Rectangle::Draw(SDL_Renderer* pRenderer) const
{
	if (!bIsFilled)
	{
		SDL_SetRenderDrawColor(pRenderer, ShapeColor.R, ShapeColor.G, ShapeColor.B, 255);
		SDL_Rect rect{ static_cast<int>(Pos.x), static_cast<int>(Pos.y), static_cast<int>(Width),
			static_cast<int>(Height) };
		SDL_RenderDrawRect(pRenderer, &rect);
	}
	else
	{
		Fill(pRenderer);
	}
}

void Rectangle::Fill(SDL_Renderer* pRenderer) const
{
	SDL_SetRenderDrawColor(pRenderer, ShapeColor.R, ShapeColor.G, ShapeColor.B, ShapeColor.A);
	SDL_Rect rect{ static_cast<int>(Pos.x), static_cast<int>(Pos.y), static_cast<int>(Width),
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

void Rectangle::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(Rectangle).name());

	writer.WriteInt("width", Width);
	writer.WriteInt("height", Height);
	writer.WriteBool("isFilled", bIsFilled);

	Shape::Serialize(writer);
}

void Rectangle::Deserialize(JsonReader const* reader)
{
	int temp = 0;
	reader->ReadInt("width", temp);
	Width = static_cast<unsigned int>(temp);

	reader->ReadInt("height", temp);
	Height = static_cast<unsigned int>(temp);

	reader->ReadBool("isFilled", bIsFilled);

	Shape::Deserialize(reader);
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
	Factory<geo::Shape>::Get().RegisterType<Line>([]() {
		return new Line{};
		});
}

void Line::Draw(SDL_Renderer* pRenderer) const
{
	SDL_SetRenderDrawColor(pRenderer, ShapeColor.R, ShapeColor.G, ShapeColor.B, 255);
	SDL_RenderDrawLine(pRenderer, static_cast<int>(Pos.x), static_cast<int>(Pos.y),
		static_cast<int>(EndPos.x), static_cast<int>(EndPos.y));
}

void Line::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(Line).name());

	writer.WriteVector("endPos", EndPos);

	Shape::Serialize(writer);
}

void Line::Deserialize(JsonReader const* reader)
{
	auto posObject = reader->ReadObject("startPos");

	posObject = reader->ReadObject("endPos");
	posObject->ReadDouble("x", EndPos.x);
	posObject->ReadDouble("y", EndPos.y);

	Shape::Deserialize(reader);
}

// Circle
Circle::Circle()
	:Shape{ShapeType::Circle}
{
}
Circle::Circle(glm::vec2 const& center, unsigned int Radius, Color const& col)
	:Shape(ShapeType::Circle, center, col),
	Radius(Radius)
{
	Factory<geo::Shape>::Get().RegisterType<Circle>([]() {
		return new Circle{};
		});
}

Circle::Circle(glm::vec2 const& center, unsigned int Radius)
	:Shape(ShapeType::Circle, center),
	Radius(Radius)
{

	Factory<geo::Shape>::Get().RegisterType<Circle>([]() {
		return new Circle{};
		});
}

void Circle::Draw(SDL_Renderer* pRenderer) const
{
	float const dAngle{ static_cast<float>(M_PI)/Radius };

	SDL_SetRenderDrawColor(pRenderer, ShapeColor.R, ShapeColor.G, ShapeColor.B, 255);
	

	for (float angle = 0.0; angle < static_cast<float>(2 * M_PI); angle += dAngle)
	{
		SDL_RenderDrawPoint(pRenderer, static_cast<int>(Pos.x + Radius * cos(angle)), 
			static_cast<int>(Pos.y + Radius * sin(angle)));
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

void Circle::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(Circle).name());

	writer.WriteInt("radius", Radius);

	Shape::Serialize(writer);
}

void Circle::Deserialize(JsonReader const* reader)
{
	int temp = 0;
	reader->ReadInt("width", temp);
	Radius = static_cast<unsigned int>(temp);

	Shape::Deserialize(reader);
}
