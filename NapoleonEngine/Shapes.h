#pragma once
#include <glm/glm.hpp>
#include "Serializer.h"

namespace geo
{
	struct Color final : public ISerializable
	{
		unsigned char R, G, B, A;

		Color() :R{}, G{}, B{}, A{} {}

		Color(unsigned char r, unsigned char g, unsigned char b)
			:R{ r },
			G{ g },
			B{ b },
			A{ 255 }
		{
		}

		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
			:R{ r },
			G{ g },
			B{ b },
			A{ a }
		{
		}

		void Print() {
			printf("R: %d G: %d B: %d A: %d \n", R, G, B, A);
		}

		void Serialize(StreamWriter& writer) const override;
		void Deserialize(JsonReader const* reader) override;
	};
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

		virtual bool IsOverlapping(Rectangle* const pRec) const { return false; }
		virtual bool IsOverlapping(Circle* const pRec) const { return false; }

	public:
		Color ShapeColor;
		glm::vec2 Pos;

		virtual void Draw(SDL_Renderer* pRenderer) const = 0;

		explicit Shape(ShapeType type, glm::vec2 const& pos = glm::vec2{}, Color const& col = Color{ 1,1,1 })
			: Type{type},
			Pos{pos},
			ShapeColor{ col } {}

		virtual ~Shape() = default;

		void Serialize(StreamWriter& writer) const override;
		void Deserialize(JsonReader const* reader) override;

		bool IsOverlapping(Shape* const other) const;

	private:
		ShapeType Type;
	};

	struct Point final : public Shape
	{
		Point();
		explicit Point(glm::vec2 const& Pos, Color const& col);

		void Draw(SDL_Renderer* pRenderer) const override;

		void Serialize(StreamWriter& writer) const override;
		void Deserialize(JsonReader const* reader) override;
	};

	struct Circle;
	struct Rectangle final : public Shape
	{
		unsigned int Width;
		unsigned int Height;
		bool bIsFilled;

		Rectangle();
		explicit Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, Color const& col, bool filled = false);
		explicit Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, bool filled = false);
		explicit Rectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool filled = false);

		void Draw(SDL_Renderer* pRenderer) const override;
		void Fill(SDL_Renderer* pRenderer) const;

		bool IsOverlapping(Rectangle* const pRec) const override;
		bool IsOverlapping(Circle* const pRec) const override;

		void Serialize(StreamWriter& writer) const override;
		void Deserialize(JsonReader const* reader) override;
	};

	struct Line final : public Shape
	{
		glm::vec2 EndPos;

		Line();
		explicit Line(glm::vec2 const& startPos, glm::vec2 const& EndPos, Color const& col);

		void Draw(SDL_Renderer* pRenderer) const override;

		void Serialize(StreamWriter& writer) const override;
		void Deserialize(JsonReader const* reader) override;
	};

	struct Circle final : public Shape
	{
		unsigned int Radius;

		Circle();
		explicit Circle(glm::vec2 const& center, unsigned int radius, Color const& col);
		explicit Circle(glm::vec2 const& center, unsigned int radius);

		void Draw(SDL_Renderer* pRenderer) const override;

		bool IsOverlapping(Rectangle* const pRec) const override;
		bool IsOverlapping(Circle* const pRec) const override;


		void Serialize(StreamWriter& writer) const override;
		void Deserialize(JsonReader const* reader) override;

	};

	bool IsPointInRec(glm::vec2 const& p, Rectangle const& rec);
	bool AreOverlapping(Rectangle const& rec1, Rectangle const& rec2);
	bool AreOverlapping(Rectangle const& rec1, Circle const& c2);
	bool AreOverlapping(Circle const& c1, Circle const& c2);
	unsigned int DistPointLine(glm::vec2 const& p, glm::vec2 const& a, glm::vec2 const& b);
}