#include "PCH.h"
#include "JsonReaderWriter.h"

#include "filereadstream.h"
#include "rapidjson.h"
#include "document.h"

#include <iostream>

//**************************************************
// Encoding Helpers
//**************************************************

static std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

std::string Base64Encode(const char* data, size_t length) {
	std::string encodedStr;
	int val = 0;
	int valb = -6;
	for (size_t i = 0; i < length; ++i) {
		val = (val << 8) + (unsigned char)data[i];
		valb += 8;
		while (valb >= 0) {
			encodedStr.push_back(base64_chars[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) {
		encodedStr.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
	}
	while (encodedStr.size() % 4) {
		encodedStr.push_back('=');  // Padding to ensure it's a multiple of 4
	}
	return encodedStr;
}

std::vector<char> Base64Decode(const std::string& base64Str) 
{
	std::vector<char> decodedData;
	int val = 0;
	int valb = -8;
	for (unsigned char c : base64Str) {
		if (isspace(c)) continue; // Skip whitespace
		if (base64_chars.find(c) == std::string::npos) break; // Ignore invalid characters

		val = (val << 6) + static_cast<int>(base64_chars.find(c));
		valb += 6;
		if (valb >= 0) {
			decodedData.push_back(static_cast<char>((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return decodedData;
}


//**************************************************
// WRITER
//**************************************************

StreamWriter::StreamWriter(StringBuffer& buffer)
	:m_BufferWriter{buffer}
{
}

void StreamWriter::WriteInt(std::string const& key, int value)
{
	if (!key.empty())
	{
		m_BufferWriter.Key(key.c_str());
	}
	
	m_BufferWriter.Int(value);
}

void StreamWriter::WriteInt64(std::string const& key, int64_t value)
{
	if (!key.empty())
	{
		m_BufferWriter.Key(key.c_str());
	}

	m_BufferWriter.Int64(value);
}

void StreamWriter::WriteBool(std::string const& key, bool value)
{
	if (!key.empty())
	{
		m_BufferWriter.Key(key.c_str());
	}

	m_BufferWriter.Bool(value);
}

void StreamWriter::WriteString(std::string const& key, std::string const& value)
{
	if (!key.empty())
	{
		m_BufferWriter.Key(key.c_str());
	}

	m_BufferWriter.String(value.c_str());
}

void StreamWriter::WriteDouble(std::string const& key, float value)
{
	if (!key.empty())
	{
		m_BufferWriter.Key(key.c_str());
	}

	m_BufferWriter.Double(value);
}

void StreamWriter::WriteDouble(std::string const& key, double value)
{
	if (!key.empty())
	{
		m_BufferWriter.Key(key.c_str());
	}

	m_BufferWriter.Double(value);
}

void StreamWriter::WriteVector(std::string const& key, glm::vec2 const& vec)
{
	if (!key.empty())
	{
		m_BufferWriter.Key(key.c_str());
	}

	m_BufferWriter.StartObject();
	WriteDouble("x", vec.x);
	WriteDouble("y", vec.y);
	m_BufferWriter.EndObject();
}


void StreamWriter::WriteBinary(std::string const& key, const char* binaryData, size_t dataLength)
{
	std::string base64Encoded = Base64Encode(binaryData, dataLength);

	m_BufferWriter.Key(key.c_str());
	m_BufferWriter.String(base64Encoded.c_str());
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

void StreamWriter::Write(std::string const& key, int value)
{
	WriteInt(key, value);
}
void StreamWriter::Write(std::string const& key, unsigned char value)
{
	WriteInt(key, value);
}
void StreamWriter::Write(std::string const& key, int64_t value)
{
	WriteInt64(key, value);
}
void StreamWriter::Write(std::string const& key, bool value)
{
	WriteBool(key, value);
}
void StreamWriter::Write(std::string const& key, std::string const& value)
{
	WriteString(key, value);
}
void StreamWriter::Write(std::string const& key, float value)
{
	WriteDouble(key, value);
}
void StreamWriter::Write(std::string const& key, double value)
{
	WriteBool(key, value);
}
void StreamWriter::Write(std::string const& key, glm::vec2 const& value)
{
	WriteVector(key, value);
}
void StreamWriter::Write(std::string const& key, glm::mat3x3 value) //assuming we serialize a transform matrix
{
	m_BufferWriter.Key(key.c_str());
	m_BufferWriter.StartObject();

	vec2 location = value[2];
	vec2 scale{};
	scale.x = glm::sqrt(glm::pow(value[0][0], 2.f) + glm::pow(value[0][1], 2.f));
	scale.y = glm::sqrt(glm::pow(value[1][0], 2.f) + glm::pow(value[1][1], 2.f));

	float rotation = glm::atan(value[0][1], value[0][0]);

	WriteVector("position", location);
	WriteVector("scale", scale);
	WriteDouble("rotation", rotation);

	m_BufferWriter.EndObject();
}

//**************************************************
// READER
//**************************************************

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

void JsonReader::ReadFloat(std::string const& attribute, float& value) const
{
	auto attributeReader = ReadAttribute(attribute);

	if (attributeReader != nullptr)
	{
		value = attributeReader->m_JsonValue->GetFloat();
	}
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
		attributeReader->ReadFloat("x", value.x);
		attributeReader->ReadFloat("y", value.y);
	}
}

void JsonReader::ReadBinary(std::string const& attribute, std::vector<char> data) const
{
	std::string base64Str;

	ReadString(attribute, base64Str);

	data = Base64Decode(base64Str);
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

void JsonReader::Read(std::string const& key, int& value) const
{
	ReadInt(key, value);
}
void JsonReader::Read(std::string const& key, uint8_t& value) const
{
	int intValue;
	ReadInt(key, intValue);


	value = static_cast<uint8_t>(intValue);
}
void JsonReader::Read(std::string const& key, int64_t& value) const
{
	ReadInt64(key, value);
}
void JsonReader::Read(std::string const& key, std::string& value) const
{
	ReadString(key, value);
}
void JsonReader::Read(std::string const& key, bool& value) const
{
	ReadBool(key, value);
}
void JsonReader::Read(std::string const& key, float& value) const
{
	ReadFloat(key, value);
}
void JsonReader::Read(std::string const& key, double& value) const
{
	ReadDouble(key, value);
}
void JsonReader::Read(std::string const& key, glm::vec2& value) const
{
	ReadVector(key, value);
}
void JsonReader::Read(std::string const& key, glm::mat3x3& value) const // assuming we deserialize a transform matrix
{
	auto attributeReader = ReadAttribute(key);

	if (attributeReader != nullptr)
	{
		vec2 location;
		vec2 scale;
		float rotation;

		attributeReader->ReadVector("position", location);
		attributeReader->ReadVector("scale", scale);

		attributeReader->ReadFloat(std::string{ "rotation" }, rotation);

		value = TransformComponent::BuildTransformMatrix(location, scale, rotation);
	}
}

void JsonReader::Read(int& value) const { value = m_JsonValue->GetInt(); }
void JsonReader::Read(uint8_t& value) const{ value = static_cast<uint8_t>(m_JsonValue->GetInt()); }
void JsonReader::Read(int64_t& value) const { value = m_JsonValue->GetInt64(); }
void JsonReader::Read(std::string& value) const { value = m_JsonValue->GetString(); }
void JsonReader::Read(bool& value) const { value = m_JsonValue->GetBool(); }
void JsonReader::Read(float& value) const { value = m_JsonValue->GetFloat(); }
void JsonReader::Read(double& value) const { value = m_JsonValue->GetDouble(); }
void JsonReader::Read(glm::vec2& value) const
{ 
	ReadFloat("x", value.x);
	ReadFloat("y", value.y);
}
void JsonReader::Read(glm::mat3x3& value) const
{
	vec2 location;
	vec2 scale;
	float rotation;

	ReadVector("position", location);
	ReadVector("scale", scale);

	ReadFloat(std::string{ "rotation" }, rotation);

	value = TransformComponent::BuildTransformMatrix(location, scale, rotation);
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

void JsonReader::Print() const
{
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	m_JsonValue->Accept(writer);

	// Print the serialized JSON
	std::cout << buffer.GetString() << std::endl;
}

std::unique_ptr<JsonReader> JsonReader::ReadAttribute(std::string const& attribute) const
{
	if (m_JsonValue->ObjectEmpty())
	{
		//LOG_WARNING("JsonReader::ReadAttribute - > Nothing to read");
		return nullptr;
	}

	if (!m_JsonValue->HasMember(attribute.c_str()))
	{
		//LOG_WARNING("JsonReader::ReadAttribute - > attribute  %s not found in current Json object", attribute.c_str());
		return nullptr;
	}

	return std::unique_ptr<JsonReader>(new JsonReader{ &(*m_JsonValue)[attribute.c_str()] });
}
