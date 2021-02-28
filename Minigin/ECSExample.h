#pragma once
#include <array>
#include <bitset>
#include <vector>

class Component;
class Entity;

using ID = unsigned int;

static unsigned int const m_MaxNbrComponents{ 32 };
using ComponentBitSet = std::bitset<m_MaxNbrComponents>;
using ComponentArray = std::array<Component*, m_MaxNbrComponents>;

inline ID GetTypeID()
{
	static ID id = 0;
	return id++;
}

template <typename T>
inline ID GetTypeID() noexcept
{
	static ID typeId = GetTypeID();
	return typeId();
}

class Component
{
public:
	Entity* m_pEntity;

	virtual ~Component();
	
	virtual void Init();
	virtual void Update();
	virtual void Draw();
};

class Entity
{
public:
	void Update()
	{
		for(auto& c : m_pComponents)
		{
			c->Update();
		}

		for (auto& c : m_pComponents)
		{
			c->Draw();
		}
	}

	void Draw(){}
	
	bool IsActive() const { return m_IsActive; }
	void Destroy() { m_IsActive = false; }

	template <typename T>
	bool  HasComponent() const
	{
		return m_CompBitSet[GetTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		m_pComponents.emplace_back(c);

		m_CompArray[GetTypeID<T>()] = c;
		m_CompBitSet[GetTypeID<T>()] = true;

		c->Init();
		return *c;
	}

	template <typename T>
	T& GetComponent() const
	{
		auto ptr(m_CompArray[GetTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
	

private:
	bool m_IsActive;
	std::vector<Component*> m_pComponents;
	ComponentArray m_CompArray;
	ComponentBitSet m_CompBitSet;
};