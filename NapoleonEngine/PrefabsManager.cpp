#include "PCH.h"
#include "PrefabsManager.h"

#include "TransformComponent.h"

#include <map>
using namespace empire;

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

	GameObject* Instantiate(std::string const& key) const;
	GameObject* Instantiate(std::string const& key, TransformComponent const& transform) const;
	GameObject* Instantiate(std::string const& key, glm::vec2 const& pos) const;
private:
	std::map<std::string, GameObject*> m_pPrefabs;
};

PrefabsManager::PrefabsManagerImpl::PrefabsManagerImpl()
	:m_pPrefabs()
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
	toReturn->GetTransform()->Translate(transform.GetPosition());
	toReturn->GetTransform()->Scale(transform.GetScale());
	toReturn->GetTransform()->Rotate(transform.GetRotation());
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
	toReturn->GetTransform()->Translate(pos);
	return toReturn;
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