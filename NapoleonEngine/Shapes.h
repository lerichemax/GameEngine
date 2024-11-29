#pragma once
#include <glm/glm.hpp>
#include "Serializer.h"

namespace geo
{
	struct Color final : public ISerializable
	{
		PROPERTY(unsigned char, R);
		PROPERTY(unsigned char, G);
		PROPERTY(unsigned char, B);
		PROPERTY(unsigned char, A);

		Color() = default;
		Color(unsigned char r, unsigned char g, unsigned char b);
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

		Color(Color const& other) = default;
		Color(Color&& other) = default;
		Color& operator=(Color const& other) = default;
		Color& operator=(Color&& other);

		void Print();

		void Serialize(StreamWriter& writer) const;
		void Deserialize(JsonReader* const reader);
	};

	SERIALIZE_CLASS(Color);

	struct Rectangle;
	struct Circle;

	struct Shape : public ISerializable
	{
	protected:
		enum class ShapeType {
			Circle,
			Rectangle,
			Other
		};

		virtual bool IsOverlapping(Rectangle* const pRec) const { pRec; return false; }
		virtual bool IsOverlapping(Circle* const pRec) const { return false; }

	public:
		PROPERTY(Color*, pShapeColor);
		PROPERTY(glm::vec2, Pos);

		virtual void Draw(SDL_Renderer* pRenderer) const = 0;

		explicit Shape(ShapeType type, glm::vec2 const& pos = glm::vec2{}, Color const& col = Color{ 1,1,1 })
			: Type{ type },
			Pos{ pos },
			pShapeColor{ new Color{ col } } {}

		virtual ~Shape() {
			delete pShapeColor;
		};

		void Serialize(StreamWriter& writer) const;
		void Deserialize(JsonReader* const reader);

		bool IsOverlapping(Shape* const other) const;

	private:
		ShapeType Type;
	};

	SERIALIZE_CLASS(Shape);

	struct Point final : public Shape
	{
		Point();
		explicit Point(glm::vec2 const& Pos, Color const& col);

		void Draw(SDL_Renderer* pRenderer) const override;

		void Serialize(StreamWriter& writer) const;
		void Deserialize(JsonReader* const reader);
	};

	struct Circle;
	struct Rectangle final : public Shape
	{
		PROPERTY(int, Width);
		PROPERTY(int, Height);
		PROPERTY(bool, bIsFilled);

		Rectangle();
		explicit Rectangle(glm::vec2 const& pos, int width, int height, Color const& col, bool filled = false);
		explicit Rectangle(glm::vec2 const& pos, int width, int height, bool filled = false);
		explicit Rectangle(int x, int y, int width, int height, bool filled = false);

		void Draw(SDL_Renderer* pRenderer) const override;
		void Fill(SDL_Renderer* pRenderer) const;

		bool IsOverlapping(Rectangle* const pRec) const override;
		bool IsOverlapping(Circle* const pRec) const override;

		void Serialize(StreamWriter& writer) const;
		void Deserialize(JsonReader* const reader);
	};

	SERIALIZE_CLASS(Rectangle);

	struct Line final : public Shape
	{
		PROPERTY(glm::vec2, EndPos);

		Line();
		explicit Line(glm::vec2 const& startPos, glm::vec2 const& EndPos, Color const& col);

		void Draw(SDL_Renderer* pRenderer) const override;

		void Serialize(StreamWriter& writer) const;
		void Deserialize(JsonReader* const reader);
	};

	SERIALIZE_CLASS(Line);

	struct Circle final : public Shape
	{
		PROPERTY(int, Radius);

		Circle();
		explicit Circle(glm::vec2 const& center, int radius, Color const& col);
		explicit Circle(glm::vec2 const& center, int radius);

		void Draw(SDL_Renderer* pRenderer) const override;

		bool IsOverlapping(Rectangle* const pRec) const override;
		bool IsOverlapping(Circle* const pRec) const override;


		void Serialize(StreamWriter& writer) const;
		void Deserialize(JsonReader* const reader);

	};

	SERIALIZE_CLASS(Circle);

	bool IsPointInRec(glm::vec2 const& p, Rectangle const& rec);
	bool AreOverlapping(Rectangle const& rec1, Rectangle const& rec2);
	bool AreOverlapping(Rectangle const& rec1, Circle const& c2);
	bool AreOverlapping(Circle const& c1, Circle const& c2);
	int DistPointLine(glm::vec2 const& p, glm::vec2 const& a, glm::vec2 const& b);
}