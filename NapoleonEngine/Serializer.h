#pragma once

#include "JsonReaderWriter.h"
#include "stream.h"

#include "writer.h"

#include "Entity.h"
#include "Reflection.h"

#include <map>

using namespace rapidjson;

class SerializationMap;

class ISerializable //eventually I want to get rid of this
{
public:
	void Serialize(StreamWriter& writer) const;
	void Deserialize(JsonReader* const reader);

};

class IContextSerializable : public ISerializable
{
public:
	virtual void Deserialize(JsonReader* const reader, SerializationMap& context) = 0;

	virtual void RestoreContext(JsonReader* const reader, SerializationMap const& context) {};

	int GetId() const { return m_Id; };

protected:
	IContextSerializable();

	PROPERTY(int, m_Id);

private:
	static int Id;
};

SERIALIZE_CLASS(IContextSerializable)

class Prefab;
class Serializer
{
public:
	std::unique_ptr<Document> Serialize(ISerializable const& serializable);
	std::unique_ptr<Document> Serialize(IContextSerializable const& serializable);
	std::unique_ptr<Document> Serialize(Prefab const& serializable);

private:
};
