#pragma once
#include "Renderer.h"
#include "Singleton.h"
#include "DebugShapes.h"
#include <glm/glm.hpp>


namespace empire
{
	class Debugger final : public Singleton<Debugger>
	{
	public:
		~Debugger();
		void Log(std::string const& message) const;
		void LogError(std::string const& message) const;
		
		void AddDebugLine(glm::vec2 const& startPoint, glm::vec2 const& endPoint, Color const& color);
		void AddDebugPoint(glm::vec2 const& pos, unsigned int thickness, Color const& color);
		void AddDebugCircle(glm::vec2 const& center, unsigned int radius, Color const& color);
	private:
		friend class Singleton<Debugger>;
		friend void Renderer::Render();
		
		Debugger();

		std::vector<Shape*> m_DebugShapes{};

		void Render(SDL_Renderer* pRenderer);
		
	};
}

