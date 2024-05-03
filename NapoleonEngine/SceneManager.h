#pragma once
#include "Singleton.h"

#include <string>
#include <memory>

class Scene;
class NapoleonEngine;
class SceneManager final : public Singleton<SceneManager>
{
public:
	~SceneManager();
	SceneManager(SceneManager const& other) = delete;
	SceneManager(SceneManager&& other) = delete;
	SceneManager& operator=(SceneManager const& rhs) = delete;
	SceneManager& operator=(SceneManager&& rhs) = delete;
		
	Scene const* GetScene(std::string const& sceneName) const;
	void AddScene(Scene* pScene);
	void RenameScene(std::string const& oldName, std::string const& newName);
	void SetSceneActive(std::string const& name);
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

	void Initialize(NapoleonEngine const* pEngine);
	void Update();
	void Render();
};
