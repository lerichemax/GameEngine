#include "JsonReaderWriter.h"
#include "Factories.h"

template<typename T>
void StreamWriter::Write(std::string const& key, T* const serializableObject)
{
	if (serializableObject == nullptr)
	{
		return; // log
	}

	m_BufferWriter.Key(key.c_str());
	Reflection::Get().SerializeClass(serializableObject, *this);
}

template<EnumType E>
void StreamWriter::Write(std::string const& key, E& enumObject)
{
	WriteInt(key, static_cast<int>(enumObject));
}

template<typename T>
void JsonReader::Read(std::string const& key, T*& serializableObject) const
{
	auto attributeReader = ReadAttribute(key);

	if (attributeReader == nullptr)
	{
		//log
		return;
	}

	if (serializableObject == nullptr)
	{
		std::string type;
		attributeReader->ReadString("type", type);

		auto pShape = Factory::Get().Create<T>(type);

		serializableObject = pShape;
	}

	Reflection::Get().DeserializeClass(serializableObject, attributeReader.get());
}

template<EnumType E>
void JsonReader::Read(std::string const& key, E& enumValue) const
{
	int intValue = -1;
	ReadInt(key, intValue);

	enumValue = static_cast<E>(intValue);
}
