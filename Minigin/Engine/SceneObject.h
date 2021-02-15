#pragma once
#include "Component.h"
#include <bitset>
#include <array>

using ID = unsigned int;



namespace dae
{
	inline ID GetTypeID()
	{
		static ID id = 0;
		return id++;
	}

	template <typename T>
	inline ID GetTypeID() noexcept
	{
		static ID typeId = GetTypeID();
		return typeId;
	}
	
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

		template <typename T>
		bool HasComponent() const;
		
		template<typename T>
		T& GetComponent() const;

		template <typename T, typename... TArgs>
		T& AddComponent(TArgs&&... mArgs);

		//template <typename T>
		//static ID GetTypeID() noexcept;
		//static ID GetTypeID();
		
		bool IsActive() const { return m_IsActive; }
		void Destroy() { m_IsActive = false; }
	
	protected:
		static unsigned int const m_MaxNbrComponents{ 32 };
		bool m_IsActive;
		
		std::vector<Component*> m_pComponents;
		std::array<Component*, m_MaxNbrComponents> m_CompArray;
		std::bitset<m_MaxNbrComponents> m_CompBitSet;
	};
	
	template <typename T>
	T& SceneObject::GetComponent() const
	{
		auto ptr(m_CompArray[GetTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	template <typename T>
	bool  SceneObject::HasComponent() const
	{
		return m_CompBitSet[GetTypeID<T>()];
	}
	
	template <typename T, typename... TArgs>
	T& SceneObject::AddComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		//c->m_pParentObject = this;
		m_pComponents.emplace_back(c);

		m_CompArray[GetTypeID<T>()] = c;
		m_CompBitSet[GetTypeID<T>()] = true;

		c->Init(this);
		return *c;
	}
}


