#include "PCH.h"
#include "PrefabsManager.h"

using namespace empire;

PrefabsManager::~PrefabsManager()
{
	for (auto pair : m_pPrefabs)
	{
		delete pair.second;
	}
	m_pPrefabs.clear();
}

void PrefabsManager::AddPrefab(std::string const& key, GameObject* pPrefab)
{
	if (m_pPrefabs.find(key) != m_pPrefabs.end())
	{
		std::cout << "Prefab already created\n";
		return;
	}
	
	m_pPrefabs.insert(std::make_pair(key, pPrefab));
}

GameObject* PrefabsManager::Instantiate(std::string const& key) const
{
	if (m_pPrefabs.find(key) == m_pPrefabs.end())
	{
		std::cout << "Prefab " << key <<" not found\n";
		return nullptr;
	}
	auto prefab = m_pPrefabs.at(key);
	auto toReturn = new GameObject(*prefab);
	
	return toReturn;
}

GameObject* PrefabsManager::Instantiate(std::string const& key, TransformComponent const& transform) const
{
	if (m_pPrefabs.find(key) == m_pPrefabs.end())
	{
		std::cout << "Prefab not found\n";
		return nullptr;
	}
	auto prefab = m_pPrefabs.at(key);
	auto toReturn = new GameObject(*prefab);
	toReturn->GetTransform()->Translate(transform.GetPosition());
	toReturn->GetTransform()->Scale(transform.GetScale());
	toReturn->GetTransform()->Rotate(transform.GetRotation());
	return toReturn;
}

GameObject* PrefabsManager::Instantiate(std::string const& key, glm::vec2 const& pos) const
{
	if (m_pPrefabs.find(key) == m_pPrefabs.end())
	{
		std::cout << "Prefab not found\n";
		return nullptr;
	}
	auto prefab = m_pPrefabs.at(key);
	auto toReturn = new GameObject(*prefab);
	toReturn->GetTransform()->Translate(pos);
	return toReturn;
}
//GameObject* PrefabsManager::Instantiate(std::string const& key, glm::vec3 const& pos, glm::vec3 const& scale) const
//{
//	
//}
//GameObject* PrefabsManager::Instantiate(std::string const& key, glm::vec3 const& pos, glm::vec3 const& scale, float rot) const
//{
//
//}