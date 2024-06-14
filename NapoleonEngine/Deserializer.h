#pragma once

class Prefab;
class JsonReader;
class Deserializer
{
public:
	std::unique_ptr<JsonReader> ReadFile(std::string const& filename);
	std::unique_ptr<JsonReader> ReadDocument(Document* const document);

	void DeserializePrefab(Document* const document, std::shared_ptr<Prefab> pPrefab);

};