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

	struct Shape
	{
		Color color;

		virtual void Draw(SDL_Renderer* pRenderer) const = 0;

		explicit Shape(Color const& col = Color{ 1,1,1 })
			: color{ col } {}

		virtual Shape* Clone() const = 0;

		virtual ~Shape() = default;

	};

	struct Point final : public Shape
	{
		glm::vec2 pos;

		explicit Point(glm::vec2 const& pos, Color const& col);
		Point* Clone() const override { return new Point(*this); }

		~Point() = default;

		void Draw(SDL_Renderer* pRenderer) const override;

	};
	struct Circle;
	struct Rectangle final : public Shape
	{
		glm::vec2 pos;
		unsigned int width;
		unsigned int height;
		bool isFilled;

		explicit Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, Color const& col, bool filled = false);
		explicit Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, bool filled = false);
		explicit Rectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool filled = false);

		Rectangle* Clone() const override { return new Rectangle(*this); }


		void Draw(SDL_Renderer* pRenderer) const override;
		void Fill(SDL_Renderer* pRenderer)const;
	};

	struct Line final : public Shape
	{
		glm::vec2 startPos;
		glm::vec2 endPos;

		explicit Line(glm::vec2 const& startPos, glm::vec2 const& endPos, Color const& col);

		Line* Clone() const override { return new Line(*this); }


		void Draw(SDL_Renderer* pRenderer) const override;


	};

	struct Circle final : public Shape
	{
		glm::vec2 center;
		unsigned int radius;

		explicit Circle(glm::vec2 const& center, unsigned int radius, Color const& col);
		explicit Circle(glm::vec2 const& center, unsigned int radius);
		Circle* Clone() const override { return new Circle(*this); }


		void Draw(SDL_Renderer* pRenderer) const override;


	};

	bool IsPointInRec(glm::vec2 const& p, Rectangle const& rec);
	bool AreOverlapping(Rectangle const& rec1, Rectangle const& rec2);
	bool AreOverlapping(Rectangle const& rec1, Circle const& c2);
	bool AreOverlapping(Circle const& c1, Circle const& c2);
	unsigned int DistPointLine(glm::vec2 const& p, glm::vec2 const& a, glm::vec2 const& b);
}