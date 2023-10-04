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
		
		unsigned int GetWindowWidth() const { return m_WindowWidth; }
		unsigned int GetWindowHeight() const { return m_WindowHeight; }
		static void Quit();
	private:
		SDL_Window* m_Window{};
		unsigned int m_WindowWidth{};
		unsigned int m_WindowHeight{};
		static bool m_bQuit;
	};
}