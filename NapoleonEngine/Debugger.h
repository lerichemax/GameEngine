#pragma once
#include <SDL_opengl.h>

#include "Renderer.h"
#include "Singleton.h"
#include "Shapes.h"
#include <glm/glm.hpp>

using namespace geo;
using namespace glm;

enum LogLevel
{
	Info, Warning, Error
};

inline void WriteLog(const char* level, const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	std::string formatStr{ std::string{level} + std::string{format} + "\n" };
	vprintf(formatStr.c_str(), arg);
	va_end(arg);
}


class Debugger final : public Singleton<Debugger>
{
public:
	~Debugger();
	void SetConsoleColor(LogLevel level) const;
		
	void DrawDebugLine(vec2 const& startPoint, vec2 const& endPoint, Color const& Color);
	void DrawDebugPoint(vec2 const& Pos, unsigned int thickness, Color const& Color);
	void DrawDebugCircle(vec2 const& center, unsigned int radius, Color const& Color);
	void DrawDebugRectangle(vec2 const& Pos, unsigned int witdth, unsigned int Height, Color const& Color);
	void DrawDebugShape(geo::Shape* const pShape);
	
private:
	friend class Singleton<Debugger>;

	friend void Renderer::Render(Registry* const pRegistry, Color const& backgroundColor);
		
	Debugger();

	HANDLE m_ConsoleHandle;

	std::vector<geo::Shape*> m_Shapes;
	std::vector<geo::Shape*> m_OwnedShapes;

	void Render(Renderer* const pRenderer);
};

#define FORMAT_LOG(level) #level " : " 

#define LOG_INFO(format, ...) Debugger::Get().SetConsoleColor(LogLevel::Info); WriteLog(FORMAT_LOG(INFO), format __VA_OPT__(,) __VA_ARGS__);
#define LOG_WARNING(format, ...) Debugger::Get().SetConsoleColor(LogLevel::Warning); WriteLog(FORMAT_LOG(WARNING), format __VA_OPT__(,) __VA_ARGS__);
#define LOG_ERROR(format, ...) Debugger::Get().SetConsoleColor(LogLevel::Error); WriteLog(FORMAT_LOG(ERROR), format __VA_OPT__(,) __VA_ARGS__);
