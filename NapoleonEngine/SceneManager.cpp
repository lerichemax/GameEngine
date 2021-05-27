#include "PCH.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace empire;

empire::SceneManager::~SceneManager()
{
	for (auto pScene : m_pScenesMap)
	{
		SafeDelete(pScene.second);
	}
}

void empire::SceneManager::Update()
{
	GetActiveScene()->Update();
}

void empire::SceneManager::Render()
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

void SceneManager::LoadScene(std::string const& name)
{
	auto scene = m_pScenesMap.at(name);
	if (scene == nullptr)
	{
		Debugger::GetInstance().Log("Wrong scene name!");
		return;
	}
	if (!scene->IsActive())
	{
		auto activeScene = GetActiveScene();
		activeScene->m_bIsActive = false;
		activeScene->CleanUpScene();
		scene->m_bIsActive = true;
	}
	else
	{
		scene->CleanUpScene();
	}
	scene->Initialize();
}

void SceneManager::ReloadCurrentScene()
{
	LoadScene(GetActiveScene()->m_Name);
}

Scene* SceneManager::GetActiveScene() const
{
	for (auto pScenePair : m_pScenesMap)
	{
		if (pScenePair.second->IsActive())
		{
			return pScenePair.second;
		}
	}
	return nullptr;
}

void SceneManager::AddScene(Scene* pScene)
{
	m_pScenesMap.insert(std::make_pair(pScene->m_Name, pScene));
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
	auto scene = GetActiveScene();
	if (scene != nullptr)
	{
		scene->m_bIsActive = false;
	}
	Renderer::GetInstance().SetBackgroundColor(0, 0, 0, 0);
	m_pScenesMap.at(name)->m_bIsActive = true;
	m_pScenesMap.at(name)->Initialize();
}