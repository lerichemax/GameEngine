#include "PCH.h"
#include "PrefabsManager.h"

#include "TransformComponent.h"
#include "Scene.h"
#include "SerializerServiceLocator.h"
#include "SerializerServiceLocator.h"
#include "Deserializer.h"

#include <map>

class Scene;
class PrefabsManager::PrefabsManagerImpl
{
public:
	PrefabsManagerImpl();
	~PrefabsManagerImpl();
	PrefabsManagerImpl(PrefabsManagerImpl const& other) = delete;
	PrefabsManagerImpl(PrefabsManagerImpl&& other) = delete;
	PrefabsManagerImpl& operator=(PrefabsManagerImpl const&) = delete;
	PrefabsManagerImpl& operator=(PrefabsManagerImpl&&) = delete;

	std::shared_ptr<Prefab> CreatePrefab();
	void SavePrefab(std::shared_ptr<Prefab> pPrefab, std::string const& name);
	void InstantiatePrefab(std::string const& name, Scene* const targetScene) const;

private:
	std::map<std::string, std::unique_ptr<rapidjson::Document>> m_Prefabs;
};

PrefabsManager::PrefabsManagerImpl::PrefabsManagerImpl()
	:m_Prefabs()
{
}

PrefabsManager::PrefabsManagerImpl::~PrefabsManagerImpl()
{
	m_Prefabs.clear();
}

std::shared_ptr<Prefab> PrefabsManager::PrefabsManagerImpl::CreatePrefab()
{

	auto pPrefab = std::make_shared<Prefab>();

	return pPrefab;
}

void PrefabsManager::PrefabsManagerImpl::SavePrefab(std::shared_ptr<Prefab> pPrefab, std::string const& name)
{
	if (m_Prefabs.find(name) != m_Prefabs.end())
	{
		Debugger::Get().LogWarning("A prefab with the name " + name + " already exists");
		return;
	}
	pPrefab->SetName(name);
	m_Prefabs.insert({ name, SerializerServiceLocator::GetSerializerService().Serialize(*pPrefab) });
}

void PrefabsManager::PrefabsManagerImpl::InstantiatePrefab(std::string const& name, Scene* const targetScene) const
{
	if (m_Prefabs.find(name) == m_Prefabs.end())
	{
		Debugger::Get().LogWarning("A prefab with the name " + name + " doesn't exists");
		return;
	}

	SerializerServiceLocator::GetDeserializerService().DeserializePrefabIntoScene(m_Prefabs.at(name).get(), targetScene);
}

PrefabsManager::PrefabsManager()
	:Singleton<PrefabsManager>(),
	m_pImpl(new PrefabsManagerImpl{})
{
}

PrefabsManager::~PrefabsManager()
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