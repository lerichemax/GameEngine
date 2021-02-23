#pragma once
#include "SceneManager.h"

namespace dae
{
	class SceneObject;
	class GameObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name, bool bIncludeFpsCounter);
	public:
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
		
		void Add(SceneObject* object);
		
		
		void Update(float deltaTime);
		void Render() const;
	private: 
		explicit Scene(const std::string& name, bool bIncludeFpsCounter);

		std::string m_Name;
		std::vector<SceneObject*> m_pObjects{};
		GameObject* m_pFpsCounter;
		
		static unsigned int m_IdCounter;

		bool m_bHasFpsCounter{};
		
		void Refresh();
		void AddFpsCounter();
	};

}
