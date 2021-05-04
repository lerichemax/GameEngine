#pragma once
#include "Singleton.h"
#include <string>
#include <glm/glm.hpp>

namespace empire
{
	class Debugger final : public Singleton<Debugger>
	{
	public:

		void Log(std::string const& message) const;
		void LogError(std::string const& message) const;
		void DrawDebugLine(glm::vec2 const& startPoint, glm::vec2 const& endPoint) const;
		void DrawDebugPoint(glm::vec2 const& pos, float thickness) const;
		void DrawDebugCircle(glm::vec2 const& center, float radius) const;
	private:
		friend class Singleton<Debugger>;
		Debugger() : Singleton(){}
	};
}

