#pragma once

namespace ecs {
	struct Component;
}

class Renderer;
class Timer;
class PrefabsManager;
class NapoleonEngine
{
public:
	virtual void RegisterScenes() const = 0;
	virtual void Cleanup();
	void Run();
		
	static void Quit();
	static NapoleonEngine* GetEngine();

	int GetWindowWidth() const;
	int GetWindowHeight() const;

protected:
	NapoleonEngine(unsigned int Width, unsigned int Height, std::string const& name, bool bCreatePrefabs);

	virtual void CreatePrefabs(std::shared_ptr<PrefabsManager> pPrefabManager) const {};

private:
	static bool m_bQuit;
	static NapoleonEngine* m_pEngine;

	std::unique_ptr<Renderer> m_pRenderer;
	std::unique_ptr<Timer> m_pTimer;
	std::shared_ptr<PrefabsManager> m_pPrefabManager;

	bool m_bCreatePrefabs;

	void StartHeapControl();
	void DumpMemoryLeaks();
};