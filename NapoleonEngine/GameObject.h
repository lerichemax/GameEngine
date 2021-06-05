#pragma once
#include "Component.h"

#include <vector>


namespace empire
{
	class Subject;
	class Observer;
	class TransformComponent;
	class Scene;
	class GameObject final
	{
		friend class Scene;
	public:
		GameObject();
		~GameObject();

		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template <class T>
		bool HasComponent() const;

		template<class T>
		T* GetComponent() const;

		template <class T>
		T* GetComponentInChildren() const;

		template <class T>
		void RemoveComponent();
		
		TransformComponent* GetTransform() const { return m_pTransform; }

		void AddComponent(Component* pComp);

		void AddChild(GameObject* pChild);
		std::vector<GameObject*> const& GetChildren() { return m_pChildren; }
		
		void SetActive(bool active);
		void Destroy();
		
		GameObject* GetParent() const{ return m_pParent; }
		Scene* const GetParentScene() const { return m_pScene; }
		
		bool IsInitialized() { return m_bIsInitialized; }
		bool IsActive() const;
		bool HasChildren() const { return m_pChildren.size() > 0; }
		std::string GetTag()const { return m_Tag; };
		
		void AddObserver(Observer* pObserver);
		void RemoveObserver(Observer* pObserver);
		void Notify(int event);
		void SetTag(std::string const& tag, bool applyToChildren);
		GameObject* FindTagInChildren(std::string const& tag);

	
	private:
		friend class PrefabsManager;
		friend class Scene;
		
		bool m_bIsActive;
		bool m_bIsDestroyed;
		bool m_bIsInitialized;
		
		std::vector<Component*> m_pComponents;
		TransformComponent* m_pTransform;
		std::vector<GameObject*> m_pChildren;
		GameObject* m_pParent;
		
		Scene* m_pScene;
		Subject* m_pSubject;

		std::string m_Tag;
		
		void Refresh();
		void Initialize();
		void Update();
		
		GameObject(const GameObject& other);
	};

	template <class T>
	T* GameObject::GetComponent() const
	{
		type_info const& type = typeid(T);
		for (Component* pObjectComp : m_pComponents)
		{
			if (typeid(*pObjectComp) == type)
			{
				return static_cast<T*>(pObjectComp);
			}
			else
			{
				T* cast = dynamic_cast<T*>(pObjectComp);
				if (cast != nullptr)
				{
					return cast;
				}
			}
		}
		return nullptr;
	}

	template <class T>
	T* GameObject::GetComponentInChildren() const
	{
		T* pComp{};
		for (auto pChild : m_pChildren)
		{
			pComp = pChild->GetComponent<T>();
			if (pComp != nullptr)
			{
				return pComp;
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

	template <class T>
	void GameObject::RemoveComponent()
	{
		T* pComp = GetComponent<T>();

		m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), pComp), m_pComponents.end());
		delete pComp;
	}
}