#include "PCH.h"
#include "PrefabsManager.h"

#include "TransformComponent.h"
#include "Scene.h"
#include "SerializerServiceLocator.h"
#include "SerializerServiceLocator.h"
#include "Deserializer.h"

#include <map>

#include "filewritestream.h"
#include "filereadstream.h"
#include "prettywriter.h"

#define PREFAB_SAVE_FOLDER "./Prefabs/"

class Scene;
class PrefabsManager::PrefabsManagerImpl
{
public:
	PrefabsManagerImpl() = default;
	~PrefabsManagerImpl() = default;
	PrefabsManagerImpl(PrefabsManagerImpl const& other) = delete;
	PrefabsManagerImpl(PrefabsManagerImpl&& other) = delete;
	PrefabsManagerImpl& operator=(PrefabsManagerImpl const&) = delete;
	PrefabsManagerImpl& operator=(PrefabsManagerImpl&&) = delete;

	std::shared_ptr<Prefab> CreatePrefab();
	void SavePrefab(std::shared_ptr<Prefab> pPrefab, std::string const& name);
	void InstantiatePrefab(std::string const& name, Scene* const targetScene) const;

private:

	void SavePrefabToFile(std::string const& name, Document* const pDoc) const;
	std::unique_ptr<rapidjson::Document> LoadPrefabFromFile(std::string const& name) const;
};

std::shared_ptr<Prefab> PrefabsManager::PrefabsManagerImpl::CreatePrefab()
{

	auto pPrefab = std::make_shared<Prefab>();

	return pPrefab;
}

void PrefabsManager::PrefabsManagerImpl::SavePrefab(std::shared_ptr<Prefab> pPrefab, std::string const& name)
{
	pPrefab->SetName(name);
	auto writtenPrefab = SerializerServiceLocator::GetSerializerService().Serialize(*pPrefab);
	SavePrefabToFile(name, writtenPrefab.get());
}

void PrefabsManager::PrefabsManagerImpl::InstantiatePrefab(std::string const& name, Scene* const targetScene) const
{
	auto pDoc = LoadPrefabFromFile(PREFAB_SAVE_FOLDER + name + ".json");

	if (!IS_VALID(pDoc))
	{
		LOG_WARNING("Couldn't create prefab %s", name.c_str());
		return;
	}

	SerializerServiceLocator::GetDeserializerService().DeserializePrefabIntoScene(pDoc.get(), targetScene);
}

void PrefabsManager::PrefabsManagerImpl::SavePrefabToFile(std::string const& name, Document* const pDoc) const
{
	std::string filename{ PREFAB_SAVE_FOLDER + name + ".json" };

	FILE* pFile;
	fopen_s(&pFile, filename.c_str(),
		"wb");

	char buffer[65536];

	// Write the JSON data to the file 
	FileWriteStream os(pFile, buffer, sizeof(buffer));
	PrettyWriter<FileWriteStream> writer(os);
	pDoc->Accept(writer);

	fclose(pFile);
}

std::unique_ptr<rapidjson::Document> PrefabsManager::PrefabsManagerImpl::LoadPrefabFromFile(std::string const& name) const
{
	FILE* pFile;

	fopen_s(&pFile, name.c_str(), "rb");

	if (!IS_VALID(pFile))
	{
		LOG_ERROR("%s not found", name.c_str());
		return nullptr;
	}

	fseek(pFile, 0, SEEK_END);

	size_t size = ftell(pFile);;
	fseek(pFile, 0, SEEK_SET);
	char* readBuffer = new char[size];
	FileReadStream is(pFile, readBuffer, sizeof(readBuffer));

	std::unique_ptr<rapidjson::Document> pDocument = std::make_unique<Document>();
	pDocument->ParseStream(is);

	delete[] readBuffer;
	fclose(pFile);

	return pDocument;
}

PrefabsManager::PrefabsManager()
	:Singleton<PrefabsManager>(),
	m_pImpl(new PrefabsManagerImpl{})
{
}

std::shared_ptr<Prefab> PrefabsManager::CreatePrefab()
{
	return m_pImpl->CreatePrefab();
}

void PrefabsManager::SavePrefab(std::shared_ptr<Prefab> pPrefab, std::string const& name)
{
	return m_pImpl->SavePrefab(pPrefab, name);
}

void PrefabsManager::InstantiatePrefab(std::string const& name, Scene* const targetScene) const
{
	m_pImpl->InstantiatePrefab(name, targetScene);
}