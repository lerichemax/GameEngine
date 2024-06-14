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

void StreamWriter::WriteIntNoKey(int value)
{
	m_BufferWriter.Int(value);
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

void StreamWriter::StartObject(std::string const& name)
{
	m_BufferWriter.Key(name.c_str());
	m_BufferWriter.StartObject();
}

void StreamWriter::StartArrayObject()
{
	m_BufferWriter.StartObject();
}

void StreamWriter::EndObject()
{
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

JsonReader::JsonReader(Document&& doc)
	:m_JsonValue{ std::make_unique<Value>(doc.GetObjectA())}
{
}

JsonReader::JsonReader(Value const& value)
	:m_JsonValue{ std::make_unique<Value>(std::move(const_cast<Value&>(value))) }
{

}

void JsonReader::ReadInt(std::string const& attribute, int& value) const
{
	Value readValue;
	if (TryReadAttribute(attribute, readValue))
	{
		value = readValue.GetInt();
	}
}
void JsonReader::ReadString(std::string const& attribute, std::string& value) const
{
	Value readValue;

	if (TryReadAttribute(attribute, readValue))
	{
		value = readValue.GetString();
	}
}
void JsonReader::ReadBool(std::string const& attribute, bool& value) const
{
	Value readValue;

	if (TryReadAttribute(attribute, readValue))
	{
		value = readValue.GetBool();
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
	Value readValue;

	if (TryReadAttribute(attribute, readValue))
	{
		value = readValue.GetDouble();
	}
}

std::unique_ptr<JsonReader> JsonReader::ReadObject(std::string const& attribute) const
{
	Value object;

	if (TryReadAttribute(attribute, object))
	{
		if (object.IsObject())
		{
			return std::unique_ptr<JsonReader>(new JsonReader{ object });
		}

		Debugger::GetInstance().LogWarning("No object found for key " + attribute);
	}
	
	return nullptr;
}

std::unique_ptr<JsonReader> JsonReader::ReadArray(std::string const& attribute) const
{
	Value array;

	if (TryReadAttribute(attribute, array))
	{
		if (array.IsArray())
		{
			return std::unique_ptr<JsonReader>(new JsonReader{ array });
		}
		Debugger::GetInstance().LogWarning("No array found for key " + attribute);
	}

	return nullptr;
}

std::unique_ptr<JsonReader> JsonReader::ReadArrayIndex(SizeType index) const
{
	if (m_JsonValue->IsArray())
	{
		return std::unique_ptr<JsonReader>(new JsonReader{ (*m_JsonValue)[index]});
	}

	Debugger::GetInstance().LogWarning("This Json object is not an array");
	return nullptr;
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

bool JsonReader::TryReadAttribute(std::string const& attribute, Value& value) const
{
	if (m_JsonValue->ObjectEmpty())
	{
		Debugger::GetInstance().LogWarning("JsonReader::ReadAttribute - > Nothing to read");
		return false;
	}

	if (!m_JsonValue->HasMember(attribute.c_str()))
	{
		Debugger::GetInstance().LogWarning("JsonReader::ReadAttribute - > attribute " + attribute + " not found in current Json object");
		return false;
	}

	value = (*m_JsonValue)[attribute.c_str()];
	return true;
}
