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
		virtual void Cleanup();
		void Run();
		
	private:
		static const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};
	};
}