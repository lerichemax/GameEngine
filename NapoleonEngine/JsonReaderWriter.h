#pragma once
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"

#include <string>

using namespace rapidjson;

class StreamWriter
{
	friend class Serializer;
public:
	StreamWriter(StringBuffer& buffer);

	void WriteInt(std::string const& key, int value);
	void WriteIntNoKey(int value);
	void WriteBool(std::string const& key, bool value);
	void WriteString(std::string const& key, std::string const& value);
	void WriteStringNoKey(std::string const& value);
	void WriteDouble(std::string const& key, float value);
	void WriteDouble(std::string const& key, double value);
	void StartObject(std::string const& name);
	void StartArrayObject();
	void EndObject();
	void StartArray(std::string const& name);
	void EndArray();

private:
	Writer<StringBuffer> m_BufferWriter;
};

class JsonReader
{
public:
	explicit JsonReader(Value* value);

	void ReadInt(std::string const& attribute, int&	value) const;
	void ReadString(std::string const& attribute, std::string& value) const;
	void ReadBool(std::string const& attribute, bool& value) const;
	void ReadDouble(std::string const& attribute, float& value) const;
	void ReadDouble(std::string const& attribute, double& value) const;
	std::unique_ptr<JsonReader> ReadObject(std::string const& attribute) const;
	std::unique_ptr<JsonReader> ReadArray(std::string const& attribute) const;
	std::unique_ptr<JsonReader> ReadArrayIndex(SizeType index) const;
	std::string ReadArrayIndexAsString(SizeType index) const;
	int ReadArrayIndexAsInt(SizeType index) const;
	SizeType GetArraySize() const;
	bool IsValid() const;

private:
	Value* const m_JsonValue;

	std::unique_ptr<JsonReader> ReadAttribute(std::string const& attribute) const;
};
