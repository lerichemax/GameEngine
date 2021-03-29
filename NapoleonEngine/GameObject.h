#pragma once
#include "Component.h"

#include <vector>

#include "TransformComponent.h"
namespace empire
{
	class TransformComponent;
	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Update(float deltaTime);

		template <class T>
		bool HasComponent() const;

		template<class T>
		T* GetComponent() const;
		TransformComponent* GetTransform() const { return m_pTransform; }

		void AddComponent(Component* pComp)
		{
			if (typeid(*pComp) == typeid(TransformComponent) && HasComponent<TransformComponent>())
			{
				std::cout << "Game object already contains a Transform component\n";
				return;
			}

			m_pComponents.emplace_back(pComp);
			pComp->Init(this);
		}

		bool IsActive() const { return m_IsActive; }
		void Destroy() { m_IsActive = false; }

	protected:
		bool m_IsActive;

		std::vector<Component*> m_pComponents;
		TransformComponent* m_pTransform;
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