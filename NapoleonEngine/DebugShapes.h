#pragma once
#include <glm/glm.hpp>

namespace empire
{
	struct Color final
	{
		unsigned char R, G, B;

		Color(unsigned char r, unsigned char g, unsigned char b)
			:R{ r },
			G{ g },
			B{ b }
		{
		}
	};

	struct Shape
	{
		Color color;

		virtual void Draw(SDL_Renderer* pRenderer) const  = 0;

		Shape(Color const& col)
			: color{ col } {}
		virtual ~Shape() = default;
	};

	struct Point final : public Shape
	{
		glm::vec2 pos;
		void Draw(SDL_Renderer* pRenderer) const override;

		Point(glm::vec2 const& pos, Color const& col);

	};

	struct Rectangle final :public Shape
	{
		glm::vec2 pos;
		unsigned int width;
		unsigned int height;

		void Draw(SDL_Renderer* pRenderer) const override;

		Rectangle(glm::vec2 const& pos, unsigned int width, unsigned int height, Color const& col);
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
