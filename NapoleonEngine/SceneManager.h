#pragma once
#include "Singleton.h"

#include <string>
#include <memory>

class Scene;
class NapoleonEngine;
class PrefabsManager;
class SceneManager final : public Singleton<SceneManager>
{
public:
	~SceneManager();
	SceneManager(SceneManager const& other) = delete;
	SceneManager(SceneManager&& other) = delete;
	SceneManager& operator=(SceneManager const& rhs) = delete;
	SceneManager& operator=(SceneManager&& rhs) = delete;
		
	void AddScene(Scene* pScene);
	void RenameScene(std::string const& oldName, std::string const& newName);
	void LoadScene(std::string const& name);
	void ReloadCurrentScene();
	Scene* GetActiveScene() const;
	
private:
	friend class Singleton<SceneManager>;
	friend class NapoleonEngine;
	friend class Renderer;

	class SceneManagerImpl;
	std::unique_ptr<SceneManagerImpl> m_pImpl;
		
	SceneManager();

	void Update();
	void SetPrefabsManagerPtr(std::weak_ptr<PrefabsManager> pPrefabsManager);
	void CleanUp();
};
