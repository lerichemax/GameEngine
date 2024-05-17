#include "PCH.h"
#include "PrefabsManager.h"

#include "TransformComponent.h"
#include "Scene.h"

#include <map>

class PrefabsManager::PrefabsManagerImpl
{
public:
	PrefabsManagerImpl();
	~PrefabsManagerImpl();
	PrefabsManagerImpl(PrefabsManagerImpl const& other) = delete;
	PrefabsManagerImpl(PrefabsManagerImpl&& other) = delete;
	PrefabsManagerImpl& operator=(PrefabsManagerImpl const&) = delete;
	PrefabsManagerImpl& operator=(PrefabsManagerImpl&&) = delete;

	std::shared_ptr<Prefab> CreatePrefab(std::string const& name);
	std::shared_ptr<Prefab> GetPrefab(std::string const& name) const;

private:
	std::map<std::string, std::shared_ptr<Prefab>> m_Prefabs;
};

PrefabsManager::PrefabsManagerImpl::PrefabsManagerImpl()
	:m_Prefabs()
{
}

PrefabsManager::PrefabsManagerImpl::~PrefabsManagerImpl()
{
	m_Prefabs.clear();
}

std::shared_ptr<Prefab> PrefabsManager::PrefabsManagerImpl::CreatePrefab(std::string const& name)
{
	if (m_Prefabs.find(name) != m_Prefabs.end())
	{
		Debugger::GetInstance().LogWarning("A prefab with the name " + name + " already exists");
		return nullptr;
	}

	auto pPrefab = std::make_shared<Prefab>(name);

	m_Prefabs.insert(std::make_pair(name, pPrefab));

	return pPrefab;
}

std::shared_ptr<Prefab> PrefabsManager::PrefabsManagerImpl::GetPrefab(std::string const& name) const
{
	return m_Prefabs.at(name);
}

PrefabsManager::PrefabsManager()
	:Singleton<PrefabsManager>(),
	m_pImpl(new PrefabsManagerImpl{})
{
	
}

PrefabsManager::~PrefabsManager()
{

}

std::shared_ptr<Prefab> PrefabsManager::CreatePrefab(std::string const& name)
{
	return m_pImpl->CreatePrefab(name);
}

std::shared_ptr<Prefab> PrefabsManager::GetPrefab(std::string const& name) const
{
	return m_pImpl->GetPrefab(name);
}