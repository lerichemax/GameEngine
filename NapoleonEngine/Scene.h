#pragma once
#include "SceneManager.h"
#include "ColliderComponent.h"

namespace empire
{
	enum class Layer;
	class SceneRenderer;
	class RendererComponent;
	class GameObject;
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
		void Update();
		void Render() const;

		bool IsActive() const { return m_bIsActive; }
		void AddToGroup(RendererComponent* pRenderer, Layer layer);
		void RemoveFromGroup(RendererComponent* pRenderer, Layer layer);
	
	private:
		friend void ColliderComponent::Initialize();
		friend ColliderComponent::~ColliderComponent();

		void AddCollider(ColliderComponent* pCollider);
		void RemoveCollider(ColliderComponent* pCollider);
		
		std::string m_Name;
		std::vector<GameObject*> m_pObjects{};
		std::vector<ColliderComponent*> m_pColliders;
		
		SceneRenderer* m_pSceneRenderer;
		static unsigned int m_IdCounter;
		
		bool m_bIsActive{};
		
		void Refresh();
		void CleanUpScene();
		void CheckCollidersCollision();
	};
}
