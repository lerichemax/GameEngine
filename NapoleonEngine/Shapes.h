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

		
		virtual ~Shape() = default;
	};

	struct Point final : public Shape
	{
		glm::vec2 pos;
		void Draw(SDL_Renderer* pRenderer) const override;

		Point(glm::vec2 const& pos, Color const& col);
	};
	struct Circle;
	struct Rectangle final :public Shape
	{
		glm::vec2 pos;
		unsigned int width;
		unsigned int height;

		void Draw(SDL_Renderer* pRenderer) const override;
		void Fill(SDL_Renderer* pRenderer)const;
		
		Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, Color const& col);
		Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height);
		Rectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
		
		bool IsOverlapping(Rectangle const& rec2);
		bool IsOverlapping(Circle const& circle);
	};

	struct Line final : public Shape
	{
		glm::vec2 startPos;
		glm::vec2 endPos;

		void Draw(SDL_Renderer* pRenderer) const override;

		Line(glm::vec2 const& startPos, glm::vec2 const& endPos, Color const& col);
	};

	struct Circle final : public Shape
	{
		glm::vec2 center;
		unsigned int radius;

		void Draw(SDL_Renderer* pRenderer) const override;

		Circle(glm::vec2 const& center, unsigned int radius, Color const& col);
	};
	
}
