#pragma once

class Scene;
class JsonReader;
class Deserializer
{
public:
	std::unique_ptr<JsonReader> ReadFile(std::string const& filename);
	std::unique_ptr<JsonReader> ReadDocument(Document* const document);

	void DeserializePrefabIntoScene(Document* const document, Scene* const targetScene);

};