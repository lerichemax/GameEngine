#include "PCH.h"
#include "Serializer.h"
#include <iostream>

#include "JsonReaderWriter.h"

int IContextSerializable::Id = 0;

std::unique_ptr<Document> Serializer::Serialize(ISerializable const& serializable)
{
	std::unique_ptr<Document> pDoc = std::make_unique<Document>();

	StringBuffer buffer;
	StreamWriter writer{ buffer };
	
	writer.m_BufferWriter.StartObject();
	serializable.Serialize(writer);

	writer.m_BufferWriter.EndObject();

	//std::cout << buffer.GetString() << std::endl;

	pDoc->Parse(buffer.GetString());

	return pDoc;
}

std::unique_ptr<Document> Serializer::Serialize(IContextSerializable const& serializable)
{
	std::unique_ptr<Document> pDoc = std::make_unique<Document>();

	StringBuffer buffer;
	StreamWriter writer{ buffer };

	writer.WriteObject(const_cast<IContextSerializable*>(&serializable)); //to change

	//std::cout << buffer.GetString() << std::endl;

	pDoc->Parse(buffer.GetString());

	return pDoc;
}

SerializationMap::SerializationMap()
{
	m_Refs.insert(std::make_pair(- 1, nullptr));
}

void SerializationMap::Add(int Id, Entity pRef)
{
	m_EntityRefs[Id] = pRef;
}

IContextSerializable::IContextSerializable()
{
	m_Id = Id++;
}