#pragma once
#include "Renderer.h"
#include "Singleton.h"
#include "Shapes.h"
#include <glm/glm.hpp>


namespace empire
{
	class Debugger final : public Singleton<Debugger>
	{
	public:
		void Log(std::string const& message) const;
		void LogError(std::string const& message) const;
		
		void DrawDebugLine(glm::vec2 const& startPoint, glm::vec2 const& endPoint, Color const& color);
		void DrawDebugPoint(glm::vec2 const& pos, unsigned int thickness, Color const& color);
		void DrawDebugCircle(glm::vec2 const& center, unsigned int radius, Color const& color);
		void DrawDebugRectangle(glm::vec2 const& pos, unsigned int witdth, unsigned int height, Color const& color);
	private:
		friend class Singleton<Debugger>;
		friend void Renderer::Render();
		
		Debugger();

		std::vector<Line> m_DebugLines{};
		std::vector<Rectangle> m_DebugRectangles;
		std::vector<Circle> m_DebugCircles;
		std::vector<Point> m_DebugPoints;

		void Render(SDL_Renderer* pRenderer);
	};
}

