#pragma once
#include <glm/glm.hpp>

namespace empire
{
	struct Color final
	{
		unsigned char R, G, B, A;

		Color(unsigned char r, unsigned char g, unsigned char b)
			:R{ r },
			G{ g },
			B{ b },
			A{255}
		{
		}

		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
			:R{ r },
			G{ g },
			B{ b },
			A{ a }
		{
		}
	};

	struct Shape
	{
		Color color;

		virtual void Draw(SDL_Renderer* pRenderer) const  = 0;

		Shape(Color const& col = Color{1,1,1})
			: color{ col } {}

		virtual Shape* Clone() const = 0;

		virtual ~Shape() = default;

	};

	struct Point final : public Shape
	{
		glm::vec2 pos;

		Point(glm::vec2 const& pos, Color const& col);
		Point* Clone() const override { return new Point(*this); }

		~Point() = default;
		
		void Draw(SDL_Renderer* pRenderer) const override;

	};
	struct Circle;
	struct Rectangle final :public Shape
	{
		glm::vec2 pos;
		unsigned int width;
		unsigned int height;
		bool isFilled;
		
		Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, Color const& col, bool filled = false);
		Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, bool filled = false);
		Rectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool filled = false);

		Rectangle* Clone() const override { return new Rectangle(*this); }

		
		void Draw(SDL_Renderer* pRenderer) const override;
		void Fill(SDL_Renderer* pRenderer)const;
		
		bool IsOverlapping(Rectangle const& rec2);
		bool IsOverlapping(Circle const& circle);

	};

	struct Line final : public Shape
	{
		glm::vec2 startPos;
		glm::vec2 endPos;

		Line(glm::vec2 const& startPos, glm::vec2 const& endPos, Color const& col);
		
		Line* Clone() const override { return new Line(*this); }

		
		void Draw(SDL_Renderer* pRenderer) const override;

		
	};

	struct Circle final : public Shape
	{
		glm::vec2 center;
		unsigned int radius;

		Circle(glm::vec2 const& center, unsigned int radius, Color const& col);
		Circle* Clone() const override { return new Circle(*this); }

		
		void Draw(SDL_Renderer* pRenderer) const override;


	};
	
}
