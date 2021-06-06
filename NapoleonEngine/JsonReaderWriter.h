#pragma once
#include "document.h"
#include <string>

namespace empire
{
	class JsonReaderWriter
	{
	public:
		explicit JsonReaderWriter(std::string const& filename);

		int ReadInt(std::string const& attribute);
		std::string ReadString(std::string const& attribute) const;
		rapidjson::Value const& ReadValue(std::string const& attribute) const;
	
	private:
		rapidjson::Document m_JsonDoc;
		FILE* m_pFile;

		rapidjson::Value const& ReadAttribute(std::string const& attribute) const;
	};
}

