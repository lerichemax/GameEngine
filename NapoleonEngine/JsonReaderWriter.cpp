#include "PCH.h"
#include "JsonReaderWriter.h"

#include "filereadstream.h"
#include "rapidjson.h"
#include "document.h"

using namespace rapidjson;
using namespace empire;

JsonReaderWriter::JsonReaderWriter(std::string const& filename)
	:m_JsonDoc(),
	m_pFile(nullptr)
{
	fopen_s(&m_pFile, filename.c_str(), "rb");

	if (m_pFile == nullptr)
	{
		Debugger::GetInstance().LogError("File " + filename + " not found");
	}
	
	fseek(m_pFile, 0, SEEK_END);

	size_t size = ftell(m_pFile);;
	fseek(m_pFile, 0, SEEK_SET);
	char* readBuffer = new char[size];
	FileReadStream is(m_pFile, readBuffer, sizeof(readBuffer));
	m_JsonDoc.ParseStream(is);
	delete[] readBuffer;
	fclose(m_pFile);
}


Value const& JsonReaderWriter::ReadAttribute(std::string const& attribute) const
{
	if (m_JsonDoc.ObjectEmpty())
	{
		Debugger::GetInstance().LogWarning("JsonReaderWriter::ReadAttribute - > No file opened, returning an empty value");
	}


	return m_JsonDoc[attribute.c_str()];
	
}

int JsonReaderWriter::ReadInt(std::string const& attribute)
{
	return ReadAttribute(attribute).GetInt();
}

std::string JsonReaderWriter::ReadString(std::string const& attribute) const
{
	return ReadAttribute(attribute).GetString();

}

rapidjson::Value const& JsonReaderWriter::ReadValue(std::string const& attribute) const
{
	return ReadAttribute(attribute);
}

