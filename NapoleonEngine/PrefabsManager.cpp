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

	void AddPrefab(std::string const& key, GameObject* pPrefab);
	std::shared_ptr<Prefab> CreatePrefab(std::string const& name);

	GameObject* Instantiate(std::string const& key) const;
	GameObject* Instantiate(std::string const& key, TransformComponent const& transform) const;
	GameObject* Instantiate(std::string const& key, glm::vec2 const& pos) const;

	std::shared_ptr<Prefab> GetPrefab(std::string const& name) const;

private:
	std::map<std::string, GameObject*> m_pPrefabs;
	std::map<std::string, std::shared_ptr<Prefab>> m_Prefabs;
};

PrefabsManager::PrefabsManagerImpl::PrefabsManagerImpl()
	:m_pPrefabs(),
	m_Prefabs()
{
}

PrefabsManager::PrefabsManagerImpl::~PrefabsManagerImpl()
{
	for (auto pair : m_pPrefabs)
	{
		delete pair.second;
	}
	m_pPrefabs.clear();
}

void PrefabsManager::PrefabsManagerImpl::AddPrefab(std::string const& key, GameObject* pPrefab)
{
	if (m_pPrefabs.find(key) != m_pPrefabs.end())
	{
		Debugger::GetInstance().LogWarning("A prefab with this name already exists");
		return;
	}

	m_pPrefabs.insert(std::make_pair(key, pPrefab));
}

std::shared_ptr<Prefab> PrefabsManager::PrefabsManagerImpl::CreatePrefab(std::string const& name)
{
	if (m_Prefabs.find(name) != m_Prefabs.end())
	{
		Debugger::GetInstance().LogWarning("A prefab with the name " + name + " already exists");
		return;
	}

	auto pPrefab = std::make_shared<Prefab>(name);

	m_Prefabs.insert(std::make_pair(name, pPrefab));

	return pPrefab;
}

GameObject* PrefabsManager::PrefabsManagerImpl::Instantiate(std::string const& key) const
{
	if (m_pPrefabs.find(key) == m_pPrefabs.end())
	{
		Debugger::GetInstance().LogWarning("Prefab " + key + " not found");
		return nullptr;
	}
	auto prefab = m_pPrefabs.at(key);
	auto toReturn = new GameObject(*prefab);

	return toReturn;
}

GameObject* PrefabsManager::PrefabsManagerImpl::Instantiate(std::string const& key, TransformComponent const& transform) const
{
	if (m_pPrefabs.find(key) == m_pPrefabs.end())
	{
		Debugger::GetInstance().LogWarning("Prefab" + key + " not found");
		return nullptr;
	}
	auto prefab = m_pPrefabs.at(key);
	auto toReturn = new GameObject(*prefab);
	toReturn->GetECSTransform()->Translate(transform.GetPosition());
	toReturn->GetECSTransform()->Scale(transform.GetScale());
	toReturn->GetECSTransform()->Rotate(transform.GetRotation());
	return toReturn;
}

GameObject* PrefabsManager::PrefabsManagerImpl::Instantiate(std::string const& key, glm::vec2 const& pos) const
{
	if (m_pPrefabs.find(key) == m_pPrefabs.end())
	{
		Debugger::GetInstance().Log("Prefab" + key + " not found");
		return nullptr;
	}
	auto prefab = m_pPrefabs.at(key);
	auto toReturn = new GameObject(*prefab);
	toReturn->GetECSTransform()->Translate(pos);
	return toReturn;
}

std::shared_ptr<Prefab> PrefabsManager::PrefabsManagerImpl::GetPrefab(std::string const& name) const
{

}

PrefabsManager::PrefabsManager()
	:Singleton<PrefabsManager>(),
	m_pImpl(new PrefabsManagerImpl{})
{
	
}

PrefabsManager::~PrefabsManager()
{

}

void PrefabsManager::AddPrefab(std::string const& key, GameObject* pPrefab)
{
	m_pImpl->AddPrefab(key, pPrefab);
}

std::shared_ptr<Prefab> PrefabsManager::CreatePrefab(std::string const& name)
{
	return m_pImpl->CreatePrefab(name);
}

GameObject* PrefabsManager::Instantiate(std::string const& key) const
{
	return m_pImpl->Instantiate(key);
}

GameObject* PrefabsManager::Instantiate(std::string const& key, TransformComponent const& transform) const
{
	return m_pImpl->Instantiate(key, transform);
}

GameObject* PrefabsManager::Instantiate(std::string const& key, glm::vec2 const& pos) const
{
	return m_pImpl->Instantiate(key, pos);
}

std::shared_ptr<Prefab> PrefabsManager::GetPrefab(std::string const& name) const
{
	return m_pImpl->GetPrefab(name);
}