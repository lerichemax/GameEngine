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
	//serializable.Serialize(writer);

	writer.m_BufferWriter.EndObject();

	pDoc->Parse(buffer.GetString());

	return pDoc;
}

std::unique_ptr<Document> Serializer::Serialize(IContextSerializable const& serializable)
{
	std::unique_ptr<Document> pDoc = std::make_unique<Document>();

	StringBuffer buffer;
	StreamWriter writer{ buffer };

	//writer.WriteObject(const_cast<IContextSerializable*>(&serializable)); //to change

	pDoc->Parse(buffer.GetString());

	return pDoc;
}

std::unique_ptr<Document> Serializer::Serialize(Prefab const& serializable) // temp ?
{
	std::unique_ptr<Document> pDoc = std::make_unique<Document>();

	StringBuffer buffer;
	StreamWriter writer{ buffer };

	writer.m_BufferWriter.StartObject();
	serializable.Serialize(writer);

	writer.m_BufferWriter.EndObject();

	pDoc->Parse(buffer.GetString());

	return pDoc;
}

IContextSerializable::IContextSerializable()
{
	m_Id = Id++;
}