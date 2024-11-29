#include "SerializationMap.h"

template <typename T>
T* SerializationMap::GetRef(int id) const
{
	auto it = m_Refs.find(id);
	if (it != m_Refs.end())
	{
		return static_cast<T*>(m_Refs.at(id));
	}
	return nullptr;
}

template <>
ecs::Entity* SerializationMap::GetRef<ecs::Entity>(int id) const
{
	auto it = m_EntityRefs.find(id);
	if (it != m_EntityRefs.end())
	{
		return const_cast<ecs::Entity*>(&it->second);
	}
	return nullptr;
}

template <typename T>
void SerializationMap::Add(int id, T* pRef)
{
	auto it = m_Refs.find(id);
	if (it == m_Refs.end())
	{
		m_Refs.insert(std::make_pair(id, pRef));
	}
	else
	{
		//Debugger::Get().Log("ID alrady added to the context");
	}
}