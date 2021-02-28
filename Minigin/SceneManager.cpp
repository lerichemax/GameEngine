#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace dae;

dae::SceneManager::~SceneManager()
{
	for (auto pScene : m_pScenesMap)
	{
		delete pScene.second;
	}
}

void dae::SceneManager::Update(float deltaTime)
{
	for(auto& scene : m_pScenesMap)
	{
		if (scene.second->IsActive())
		{
			scene.second->Update(deltaTime);
			break;
		}
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_pScenesMap)
	{
		if (scene.second->IsActive())
		{
			scene.second->Render();
			break;
		}
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name, bool bIncludeFpsCounter)
{
	const auto pScene = new Scene{ name, bIncludeFpsCounter };
	pScene->m_bIsActive = true;
	m_pScenesMap.insert({ name, pScene });
	return *pScene;
}

void SceneManager::RenameScene(std::string const& oldName, std::string const& newName)
{
	auto node = m_pScenesMap.find(oldName);

	if (node != m_pScenesMap.end())
	{
		auto const value = m_pScenesMap.at(oldName);
		m_pScenesMap.erase(oldName);
		m_pScenesMap.insert({ newName, value });
		value->m_Name = newName;
	}
}

void SceneManager::SetSceneActive(std::string const& name)
{
	for (auto & pair : m_pScenesMap)
	{
		pair.second->m_bIsActive = false;
	}

	m_pScenesMap.at(name)->m_bIsActive = true;
}