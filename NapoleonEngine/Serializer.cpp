#include "PCH.h"
#include "Serializer.h"
#include <iostream>

#include "JsonReaderWriter.h"

int IContextSerializable::Id = 0;

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
	:m_Id{Id++}
{
}

IContextSerializable::IContextSerializable(IContextSerializable const& other)
	:m_Id{Id++}
{

}

IContextSerializable::IContextSerializable(IContextSerializable&& other)
	:m_Id{other.m_Id}
{
	other.m_Id = -1;
}

IContextSerializable& IContextSerializable::operator=(IContextSerializable const& other)
{
	m_Id = Id++;

	return *this;
}

IContextSerializable& IContextSerializable::operator=(IContextSerializable&& other)
{
	m_Id = other.m_Id;

	other.m_Id = -1;

	return *this;
}