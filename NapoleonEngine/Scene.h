#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace empire
{
	
	class Scene
	{
		friend class SceneManager;
	public:
		explicit Scene(const std::string& name);
		virtual ~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		virtual void Initialize() = 0;
		
		void Add(GameObject* object);
		void Update(float deltaTime);
		void Render() const;

		bool IsActive() const { return m_bIsActive; }
	private: 
		std::string m_Name;
		std::vector<GameObject*> m_pObjects{};
		
		static unsigned int m_IdCounter;
		
		bool m_bIsActive{};
		
		void Refresh();
		void CleanUpScene();
	};

}
