#include "PCH.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "JsonReaderWriter.h"
#include "Scene.h"

#include "filereadstream.h"
#include "rapidjson.h"
#include "document.h"

std::unique_ptr<JsonReader> Deserializer::ReadFile(std::string const& filename)
{
	FILE* file;
	fopen_s(&file, filename.c_str(), "rb");

	if (file == nullptr)
	{
		Debugger::GetInstance().LogError("File " + filename + " not found");
	}

	fseek(file, 0, SEEK_END);

	size_t size = ftell(file);;
	fseek(file, 0, SEEK_SET);
	char* readBuffer = new char[size];
	FileReadStream is(file, readBuffer, sizeof(readBuffer));

	Document doc;
	doc.ParseStream(is);

	delete[] readBuffer;
	fclose(file);

	return std::make_unique<JsonReader>(JsonReader{ std::move(doc) });
}

std::unique_ptr<JsonReader> Deserializer::ReadDocument(Document* const document)
{
	return std::make_unique<JsonReader>(JsonReader{ std::move(*document) });
}

void Deserializer::DeserializePrefab(Document* const document, std::shared_ptr<Prefab> pPrefab)
{
	SerializationMap context;

	auto doc = ReadDocument(document);
	pPrefab->Deserialize(doc.get(), context);
	pPrefab->RestoreContext(doc.get(), context);
}