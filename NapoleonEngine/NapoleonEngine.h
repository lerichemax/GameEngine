#pragma once

namespace ecs {
	struct Component;
}

template <class T>
class Factory;

class Renderer;
class SingletonWrapper;
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

	void RegisterSingleton(SingletonWrapper* singleton);
	unsigned int GetWindowWidth() const;
	unsigned int GetWindowHeight() const;

protected:
	NapoleonEngine(unsigned int Width, unsigned int Height, std::string const& name, bool bCreatePrefabs);

	virtual void CreatePrefabs() const = 0;
	virtual void RegisterComponentsToFactory_Imp(Factory<ecs::Component>& factory) const = 0; //Eventually I want to get rid of this

private:
	static bool m_bQuit;
	static NapoleonEngine* m_pEngine;

	std::vector<SingletonWrapper*> m_Singletons;

	std::unique_ptr<Renderer> m_pRenderer;
	std::unique_ptr<Timer> m_pTimer;

	bool m_bCreatePrefabs;

	void CreateBasePrefabs();
	void RegisterComponentsToFactory() const;
	void StartHeapControl();
	void DumpMemoryLeaks();
};