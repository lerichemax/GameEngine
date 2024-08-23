#pragma once

#include "JsonReaderWriter.h"
#include "Debugger.h"
#include "stream.h"

#include "writer.h"

#include <map>

using namespace rapidjson;

class ISerializable
{
public:
	virtual void Serialize(StreamWriter& writer) const = 0;
	virtual void Deserialize(JsonReader const* reader) = 0;

};

class SerializationMap final
{
public:
	template <typename T> void Add(int Id, T* pRef);
	template <typename T> T* GetRef(int id) const;

private:
	friend class Serializer;
	friend class Deserializer;

	SerializationMap();

	std::map<int, void*> m_Refs{};
};

class IContextSerializable
{
public:
	virtual void Serialize(StreamWriter& writer) const = 0;
	virtual void Deserialize(JsonReader const* reader, SerializationMap& context) = 0;

	virtual void RestoreContext(JsonReader const* reader, SerializationMap const& context) {};

	int GetId() const { return m_Id; };

protected:
	IContextSerializable();

	int m_Id;

private:
	static int Id;
};

class Serializer
{
public:
	std::unique_ptr<Document> Serialize(ISerializable const& serializable);
	std::unique_ptr<Document> Serialize(IContextSerializable const& serializable);

private:
};

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
		//Debugger::GetInstance().LogWarning("ID alrady added to the context");
	}
}