#pragma once

#include "stream.h"

#include "writer.h"

#include "Entity.h"
#include "Reflection.h"

#include <map>

using namespace rapidjson;

class SerializationMap;

class IContextSerializable
{
public:
	IContextSerializable();
	IContextSerializable(IContextSerializable const& other);
	IContextSerializable(IContextSerializable&& other);
	IContextSerializable& operator=(IContextSerializable const& other);
	IContextSerializable& operator=(IContextSerializable&& other);

	int GetId() const { return m_Id; };

	virtual bool test() { return true; } //TEMP, needed for reflection

protected:
	PROPERTY(int, m_Id);

private:
	static int Id;
};

SERIALIZE_CLASS(IContextSerializable)

class Prefab;
class Serializer
{
public:
	std::unique_ptr<Document> Serialize(IContextSerializable const& serializable);
	std::unique_ptr<Document> Serialize(Prefab const& serializable);
};
