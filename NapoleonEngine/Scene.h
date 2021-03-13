#pragma once
#include "SceneManager.h"

namespace empire
{
	class SceneObject;
	class GameObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name, bool bIncludeFpsCounter);
		friend void SceneManager::RenameScene(std::string const& oldName, std::string const& newName);
		friend void SceneManager::SetSceneActive(std::string const& name);
	public:
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
		
		void Add(SceneObject* object);
		
		void Update(float deltaTime);
		void Render() const;

		bool IsActive() const { return m_bIsActive; }
	private: 
		explicit Scene(const std::string& name, bool bIncludeFpsCounter);

		std::string m_Name;
		std::vector<SceneObject*> m_pObjects{};
		GameObject* m_pFpsCounter;
		
		static unsigned int m_IdCounter;

		bool m_bHasFpsCounter{};
		bool m_bIsActive{};
		
		void Refresh();
		void AddFpsCounter();
		
	};

}
