#pragma once

namespace ecs {
	struct Component;
}

class Renderer;
class Timer;
class Scene;
class NapoleonEngine
{
public:
	virtual void InitGame() const = 0;
	virtual void Cleanup();
	void Run();
		
	static void Quit();
	static NapoleonEngine* GetEngine();

	int GetWindowWidth() const;
	int GetWindowHeight() const;

protected:
	NapoleonEngine(unsigned int Width, unsigned int Height, std::string const& name, bool bCreatePrefabs);

	virtual void CreatePrefabs() const {};

private:
	static bool m_bQuit;
	static NapoleonEngine* m_pEngine;

	std::unique_ptr<Renderer> m_pRenderer;
	std::unique_ptr<Timer> m_pTimer;

	bool m_bCreatePrefabs;

	void StartHeapControl();
	void DumpMemoryLeaks();
};