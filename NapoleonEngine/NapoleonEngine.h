#pragma once

struct SDL_Window;
namespace empire
{
	class GameObject;
	class NapoleonEngine
	{
	public:
		void Initialize();
		virtual void LoadGame() const = 0;
		virtual void CreatePrefabs() const = 0;
		virtual void Cleanup();
		void Run();
		
		int GetWindowWidth() { return m_WindowWidth; }
		int GetWindowHeight() { return m_WindowHeight; }
	private:
		SDL_Window* m_Window{};
		int m_WindowWidth;
		int m_WindowHeight;
	};
}