#pragma once
#include "Singleton.h"
#include <map>
#include <string>
namespace empire
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name, bool bIncludeFpsCounter = false);
		~SceneManager();
		void Update(float deltaTime);
		void Render();
		Scene const* GetScene(std::string const& sceneName) const { return m_pScenesMap.at(sceneName); }
		void RenameScene(std::string const& oldName, std::string const& newName);
		void SetSceneActive(std::string const& name);
		
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::map <std::string, Scene*> m_pScenesMap;
	};
}
