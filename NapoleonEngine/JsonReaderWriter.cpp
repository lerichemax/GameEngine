#include "PCH.h"
#include "JsonReaderWriter.h"

#include "filereadstream.h"
#include "rapidjson.h"
#include "document.h"


StreamWriter::StreamWriter(StringBuffer& buffer)
	:m_BufferWriter{buffer}
{
}

void StreamWriter::WriteInt(std::string const& key, int value)
{
	m_BufferWriter.Key(key.c_str());
	m_BufferWriter.Int(value);
}

void StreamWriter::WriteInt64(std::string const& key, int64_t value)
{
	m_BufferWriter.Key(key.c_str());
	m_BufferWriter.Int64(value);
}

void StreamWriter::WriteIntNoKey(int value)
{
	m_BufferWriter.Int(value);
}

void StreamWriter::WriteIntNoKey(int64_t value)
{
	m_BufferWriter.Int64(value);
}

void StreamWriter::WriteBool(std::string const& key, bool value)
{
	m_BufferWriter.Key(key.c_str());
	m_BufferWriter.Bool(value);
}

void StreamWriter::WriteString(std::string const& key, std::string const& value)
{
	m_BufferWriter.Key(key.c_str());
	m_BufferWriter.String(value.c_str());
}

void StreamWriter::WriteStringNoKey(std::string const& value)
{
	m_BufferWriter.String(value.c_str());
}

void StreamWriter::WriteDouble(std::string const& key, float value)
{
	m_BufferWriter.Key(key.c_str());
	m_BufferWriter.Double(value);
}

void StreamWriter::WriteDouble(std::string const& key, double value)
{
	m_BufferWriter.Key(key.c_str());
	m_BufferWriter.Double(value);
}

void StreamWriter::WriteVector(std::string const& key, glm::vec2 const& vec)
{
	m_BufferWriter.Key(key.c_str());
	m_BufferWriter.StartObject();
	WriteDouble("x", vec.x);
	WriteDouble("y", vec.y);
	m_BufferWriter.EndObject();
}

void StreamWriter::WriteObject(std::string const& name, ISerializable* const serializableObject)
{
	m_BufferWriter.Key(name.c_str());
	m_BufferWriter.StartObject();
	serializableObject->Serialize(*this);
	m_BufferWriter.EndObject();
}

void StreamWriter::WriteObject(std::string const& name, ISerializable const* const serializableObject)
{
	m_BufferWriter.Key(name.c_str());
	m_BufferWriter.StartObject();
	serializableObject->Serialize(*this);
	m_BufferWriter.EndObject();
}

void StreamWriter::WriteObject(ISerializable* const serializableObject)
{
	m_BufferWriter.StartObject();
	serializableObject->Serialize(*this);
	m_BufferWriter.EndObject();
}

void StreamWriter::StartArray(std::string const& name)
{
	m_BufferWriter.Key(name.c_str());
	m_BufferWriter.StartArray();
}

void StreamWriter::EndArray()
{
	m_BufferWriter.EndArray();
}

JsonReader::JsonReader(Value* value)
	:m_JsonValue{ value }
{
}

void JsonReader::ReadInt(std::string const& attribute, int& value) const
{
	auto attributeReader = ReadAttribute(attribute);

	if (attributeReader != nullptr)
	{
		value = attributeReader->m_JsonValue->GetInt();
	}

}

void JsonReader::ReadInt64(std::string const& attribute, int64_t& value) const
{
	auto attributeReader = ReadAttribute(attribute);

	if (attributeReader != nullptr)
	{
		value = attributeReader->m_JsonValue->GetInt64();
	}
}
void JsonReader::ReadString(std::string const& attribute, std::string& value) const
{
	auto attributeReader = ReadAttribute(attribute);

	if (attributeReader != nullptr)
	{
		value = attributeReader->m_JsonValue->GetString();
	}
}
void JsonReader::ReadBool(std::string const& attribute, bool& value) const
{
	auto attributeReader = ReadAttribute(attribute);

	if (attributeReader != nullptr)
	{
		value = attributeReader->m_JsonValue->GetBool();
	}
	else
	{ 
		value = false;
	}
}

void JsonReader::ReadDouble(std::string const& attribute, float& value) const
{
	double valueDouble = 0;

	ReadDouble(attribute, valueDouble);

	value = static_cast<float>(valueDouble);
}

void JsonReader::ReadDouble(std::string const& attribute, double& value) const
{
	auto attributeReader = ReadAttribute(attribute);

	if (attributeReader != nullptr)
	{
		value = attributeReader->m_JsonValue->GetDouble();
	}
}

void JsonReader::ReadVector(std::string const& attribute, glm::vec2& value) const
{
	auto attributeReader = ReadAttribute(attribute);

	if (attributeReader != nullptr)
	{
		attributeReader->ReadDouble("x", value.x);
		attributeReader->ReadDouble("y", value.y);
	}
}

std::unique_ptr<JsonReader> JsonReader::ReadObject(std::string const& attribute) const
{
	auto attributeReader = ReadAttribute(attribute);

	if (attributeReader != nullptr)
	{
		if (attributeReader->m_JsonValue->IsObject())
		{
			return attributeReader;
		}
		LOG_WARNING("No object found for key %s", attribute.c_str());
	}
	
	return nullptr;
}

std::unique_ptr<JsonReader> JsonReader::ReadArray(std::string const& attribute) const
{
	auto attributeReader = ReadAttribute(attribute);

	if (attributeReader != nullptr)
	{
		if (attributeReader->m_JsonValue->IsArray())
		{
			return attributeReader;
		}
		LOG_WARNING("No array found for key %s", attribute.c_str());
	}

	return nullptr;
}

std::unique_ptr<JsonReader> JsonReader::ReadArrayIndex(SizeType index) const
{
	if (m_JsonValue->IsArray())
	{
		return std::unique_ptr<JsonReader>(new JsonReader{ &(*m_JsonValue)[index]});
	}

	LOG_WARNING("This Json object is not an array");
	return nullptr;
}

std::unique_ptr<JsonReader> JsonReader::ReadArrayIndex(size_t index) const
{
	return ReadArrayIndex(static_cast<SizeType>(index));
}

std::string JsonReader::ReadArrayIndexAsString(SizeType index) const
{
	if (m_JsonValue->IsArray())
	{
		return (*m_JsonValue)[index].GetString();
	}

	return "";
}

int JsonReader::ReadArrayIndexAsInt(SizeType index) const
{
	if (m_JsonValue->IsArray())
	{
		return (*m_JsonValue)[index].GetInt();
	}

	return -1;
}

int64_t JsonReader::ReadArrayIndexAsInt64(SizeType index) const
{
	if (m_JsonValue->IsArray())
	{
		return (*m_JsonValue)[index].GetInt64();
	}

	return -1;
}

SizeType JsonReader::GetArraySize() const
{
	if (m_JsonValue->IsArray())
	{
		return m_JsonValue->Size();
	}

	return 0;
}

bool JsonReader::IsValid() const
{
	return !m_JsonValue->IsNull();
}

std::unique_ptr<JsonReader> JsonReader::ReadAttribute(std::string const& attribute) const
{
	if (m_JsonValue->ObjectEmpty())
	{
		LOG_WARNING("JsonReader::ReadAttribute - > Nothing to read");
		return nullptr;
	}

	if (!m_JsonValue->HasMember(attribute.c_str()))
	{
		LOG_WARNING("JsonReader::ReadAttribute - > attribute  %s not found in current Json object", attribute.c_str());
		return nullptr;
	}

	return std::unique_ptr<JsonReader>(new JsonReader{ &(*m_JsonValue)[attribute.c_str()] });
}
