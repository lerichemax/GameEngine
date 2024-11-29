#pragma once
#include "Entity.h"

#include <map>

class SerializationMap final
{
public:
	SerializationMap();//temp, should be private
	void Add(int Id, ecs::Entity pRef);

	template <typename T> void Add(int Id, T* pRef);
	template <typename T> T* GetRef(int id) const;
	template <> inline ecs::Entity* GetRef<ecs::Entity>(int id) const;

private:
	friend class Serializer;
	friend class Deserializer;

	//SerializationMap();

	std::map<int, void*> m_Refs{};
	std::map<int, ecs::Entity> m_EntityRefs{};

};

#include "SerializationMap.inl"