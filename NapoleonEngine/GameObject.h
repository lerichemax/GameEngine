#pragma once
#include "Component.h"

#include <vector>

#include "TransformComponent.h"
#include "Scene.h"

namespace empire
{
	
	class TransformComponent;
	class GameObject
	{
		friend class Scene;
	public:
		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Update();

		template <class T>
		bool HasComponent() const;

		template<class T>
		T* GetComponent() const;
		TransformComponent* GetTransform() const { return m_pTransform; }

		void AddComponent(Component* pComp);

		void AddChild(GameObject* pChild);
		std::vector<GameObject*> const& GetChildren() { return m_pChildren; }
		//GameObject* GetParent() const { return m_pParent; }
		
		bool HasChildren() const { return m_pChildren.size() > 0; }
		
		bool IsActive() const { return m_IsActive; }
		void Destroy() { m_IsActive = false; }
		void Render() const;
		Scene* const GetParentScene() const { return m_pScene; }
	
	private:
		
		bool m_IsActive;

		std::vector<Component*> m_pComponents;
		TransformComponent* m_pTransform;
		std::vector<GameObject*> m_pChildren;
		//GameObject* m_pParent;
		
		Scene* m_pScene;

		void Refresh();
		void Initialize();
	};

	template <class T>
	T* GameObject::GetComponent() const
	{
		auto const& type = typeid(T);

		for (Component* pObjectComp : m_pComponents)
		{
			if (typeid(*pObjectComp) == type)
			{
				return static_cast<T*>(pObjectComp);
			}
		}
		return nullptr;
	}

	template <class T>
	bool  GameObject::HasComponent() const
	{
		auto const& type = typeid(T);

		for (Component* pObjectComp : m_pComponents)
		{
			if (typeid(*pObjectComp) == type)
			{
				return true;
			}
		}
		return false;
	}
}