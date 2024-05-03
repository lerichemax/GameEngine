#pragma once

#include <memory>

struct SDL_Window;
class GameObject;
class SystemManager;
class NapoleonEngine
{
public:
	virtual void InitGame() const = 0;
	virtual void CreatePrefabs() const = 0;
	virtual void Cleanup();
	void Run();
		
	static void Quit();

protected:
	void Initialize(unsigned int width, unsigned int height, std::string const& name);

private:
	static bool m_bQuit;
};