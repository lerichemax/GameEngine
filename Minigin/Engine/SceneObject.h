#pragma once
#include "Component.h"
#include "Transform.h"

namespace dae
{
	class SceneObject
	{
	public:
		SceneObject();
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject & other) = delete;
		SceneObject(SceneObject && other) = delete;
		SceneObject& operator=(const SceneObject & other) = delete;
		SceneObject& operator=(SceneObject && other) = delete;
		
		virtual void Update() = 0;

		void SetPosition(float x, float y);
		Transform const& GetPosition() const { return m_Transform; }
		
		template<typename T>
		T* GetComponent() const;

		template<typename T>
		void AddComponent(T*);
	protected:
		Transform m_Transform;
		std::vector<Component*> m_pComponents;
	};
	
	template<typename T>
	T* SceneObject::GetComponent() const
	{
		for (Component* pComp : m_pComponents)
		{
			T* pT = dynamic_cast<T*>(pComp);
			if (pT != nullptr)
			{
				return pT;
			}
		}
		return nullptr;
	}

	template<typename T>
	void SceneObject::AddComponent(T* pT)
	{
		m_pComponents.push_back(pT);
	}
}


