#pragma once
#include "Singleton.h"
#include <map>
#include <string>
namespace empire
{
	class Scene;
	class NapoleonEngine;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		//Scene& CreateScene(const std::string& name);
		~SceneManager();
		
		Scene const* GetScene(std::string const& sceneName) const { return m_pScenesMap.at(sceneName); }
		void AddScene(Scene* pScene);
		void RenameScene(std::string const& oldName, std::string const& newName);
		void SetSceneActive(std::string const& name);
		void LoadScene(std::string const& name);
		void ReloadCurrentScene();
		Scene* GetActiveScene() const;
		NapoleonEngine const*  GetEngine()const { return m_pEngine; }
	private:
		friend class Singleton<SceneManager>;
		friend class NapoleonEngine;
		friend class Renderer;
		SceneManager();

		Scene* m_pNextActiveScene;

		Scene* m_pActiveScene;
		NapoleonEngine const*  m_pEngine;
		std::map <std::string, Scene*> m_pScenesMap;
		void Initialize(NapoleonEngine const* pEngine);
		void Update();
		void Render();
	};
}
