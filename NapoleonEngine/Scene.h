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
		
		void AddObject(GameObject* object);
		void Update();
		void Render() const;

		bool IsActive() const { return m_bIsActive; }
		void AddToGroup(RendererComponent* pRenderer, Layer layer);
		void RemoveFromGroup(RendererComponent* pRenderer, Layer layer);

		std::string GetName()const { return m_Name; }
	private:
		friend void ColliderComponent::Initialize();
		friend ColliderComponent::~ColliderComponent();
		friend class SceneManager;
		
		void AddCollider(ColliderComponent* pCollider);
		void RemoveCollider(ColliderComponent* pCollider);
		
		std::string m_Name;
		std::vector<GameObject*> m_pObjects{};
		std::vector<ColliderComponent*> m_pColliders;
		
		SceneRenderer* m_pSceneRenderer;
		
		bool m_bIsActive;
		bool m_bIsInitialized;
		
		void Refresh();
		void CleanUpScene();
		void CheckCollidersCollision();
	};
}
