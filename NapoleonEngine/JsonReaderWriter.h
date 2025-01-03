#pragma once
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"

#include "SerializationMap.h" // remove later

#include <string>
#include <concepts>
#include <type_traits>
#include <glm/vec2.hpp>

template <typename T>
concept EnumType = std::is_enum<T>::value;

using namespace rapidjson;

namespace ecs
{
	class Registry;
}

class Font;
class Texture2D;
class StreamWriter
{
	friend class Serializer;
	friend class ecs::Registry;
	friend class Reflection;
public:
	StreamWriter(StringBuffer& buffer);

	void WriteInt(std::string const& key, int value);
	void WriteInt64(std::string const& key, int64_t value);
	void WriteIntNoKey(int value);
	void WriteIntNoKey(int64_t value);
	void WriteBool(std::string const& key, bool value);
	void WriteString(std::string const& key, std::string const& value);
	void WriteStringNoKey(std::string const& value);
	void WriteDouble(std::string const& key, float value);
	void WriteDouble(std::string const& key, double value);
	void WriteVector(std::string const& key, glm::vec2 const& vec);
	void WriteBinary(std::string const& key, const char* binaryData, size_t dataLength);

	void StartArray(std::string const& name);
	void EndArray();

	void Write(std::string const& key, int value);
	void Write(std::string const& key, unsigned char value);
	void Write(std::string const& key, int64_t value);
	void Write(std::string const& key, bool value);
	void Write(std::string const& key, std::string const& value);
	void Write(std::string const& key, float value);
	void Write(std::string const& key, double value);
	void Write(std::string const& key, glm::vec2 const& value);
	void Write(std::string const& key, glm::mat3x3 value);
	template<typename T> void Write(std::string const& key, T* const serializableObject);
	template<EnumType E> void Write(std::string const& key, E& enumObject);

private:
	Writer<StringBuffer> m_BufferWriter;
};


class JsonReader
{
public:
	explicit JsonReader(Value* value);

	void ReadInt(std::string const& attribute, int&	value) const;
	void ReadInt64(std::string const& attribute, int64_t&	value) const;
	void ReadString(std::string const& attribute, std::string& value) const;
	void ReadBool(std::string const& attribute, bool& value) const;
	void ReadDouble(std::string const& attribute, float& value) const;
	void ReadDouble(std::string const& attribute, double& value) const;
	void ReadVector(std::string const& attribute, glm::vec2& value) const;
	void ReadBinary(std::string const& attribute, std::vector<char> data) const;
	std::unique_ptr<JsonReader> ReadObject(std::string const& attribute) const;
	std::unique_ptr<JsonReader> ReadArray(std::string const& attribute) const;
	std::unique_ptr<JsonReader> ReadArrayIndex(SizeType index) const;
	std::unique_ptr<JsonReader> ReadArrayIndex(size_t index) const;
	std::string ReadArrayIndexAsString(SizeType index) const;
	int ReadArrayIndexAsInt(SizeType index) const;
	int64_t ReadArrayIndexAsInt64(SizeType index) const;

	void Read(std::string const& key, int& value) const;
	void Read(std::string const& key, uint8_t& value) const;
	void Read(std::string const& key, int64_t& value) const;
	void Read(std::string const& key, std::string& value) const;
	void Read(std::string const& key, bool& value) const;
	void Read(std::string const& key, float& value) const;
	void Read(std::string const& key, double& value) const;
	void Read(std::string const& key, glm::vec2& value) const;
	void Read(std::string const& key, glm::mat3x3& value) const;
	template<typename T> void Read(std::string const& key, T*& serializableObject) const;
	template<EnumType E> void Read(std::string const& key, E& enumValue) const;

	SizeType GetArraySize() const;
	bool IsValid() const;
	void Print() const;

private:
	Value* const m_JsonValue;

	std::unique_ptr<JsonReader> ReadAttribute(std::string const& attribute) const;
};

#include "JsonReaderWriter.inl"