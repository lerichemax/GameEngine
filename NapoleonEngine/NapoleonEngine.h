#pragma once

class SingletonWrapper;
class NapoleonEngine
{
public:
	virtual void InitGame() const = 0;
	virtual void Cleanup();
	void Run();
		
	static void Quit();
	static NapoleonEngine* GetEngine();

	void RegisterSingleton(SingletonWrapper* singleton);

protected:
	NapoleonEngine();

	void Initialize(unsigned int Width, unsigned int Height, std::string const& name);
	virtual void CreatePrefabs() const = 0;

private:
	static bool m_bQuit;
	static NapoleonEngine* m_pEngine;

	std::vector<SingletonWrapper*> m_Singletons;

	void CreateBasePrefabs();
};