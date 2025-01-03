#include "JsonReaderWriter.h"
#include "Factories.h"
#include "ResourceManager.h"

#include <type_traits>

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
		T* pObj = nullptr;

		if (!type.empty())
		{
			pObj = Factory::Get().Create<T>(type);
		}

		serializableObject = pObj;
	}

	Reflection::Get().DeserializeClass(serializableObject, attributeReader.get());
}

template<>
inline void JsonReader::Read<Font>(std::string const& key, Font*& serializableObject) const
{
	auto attributeReader = ReadAttribute(key);

	if (attributeReader == nullptr)
	{
		//log
		return;
	}

	if (serializableObject == nullptr)
	{
		std::string filePath;
		attributeReader->ReadString("m_FilePath", filePath);
		
		int size;
		attributeReader->ReadInt("m_Size", size);

		serializableObject = ResourceManager::Get().GetFont(filePath, size);
	}
}

template<>
inline void JsonReader::Read<Texture2D>(std::string const& key, Texture2D*& serializableObject) const
{
	auto attributeReader = ReadAttribute(key);

	if (attributeReader == nullptr)
	{
		//log
		return;
	}

	if (serializableObject == nullptr)
	{
		std::string filePath;
		attributeReader->ReadString("m_FilePath", filePath);


		serializableObject = ResourceManager::Get().GetTexture(filePath);
	}
}

template<EnumType E>
void JsonReader::Read(std::string const& key, E& enumValue) const
{
	int intValue = -1;
	ReadInt(key, intValue);

	enumValue = static_cast<E>(intValue);
}
