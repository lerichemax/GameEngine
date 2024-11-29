#include "PCH.h"
#include "SerializationMap.h"

SerializationMap::SerializationMap()
{
	m_Refs.insert(std::make_pair(-1, nullptr));
}

void SerializationMap::Add(int Id, Entity pRef)
{
	m_EntityRefs[Id] = pRef;
}