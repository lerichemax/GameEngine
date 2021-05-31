#include "PCH.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace empire;

empire::SceneManager::SceneManager()
	:m_pScenesMap(),
	m_pEngine(nullptr),
	m_pActiveScene(nullptr)
{
	
}
empire::SceneManager::~SceneManager()
{
	for (auto pScene : m_pScenesMap)
	{
		SafeDelete(pScene.second);
	}
}

void empire::SceneManager::Initialize(NapoleonEngine const* pEngine)
{
	m_pEngine = pEngine;
	for(auto pScene : m_pScenesMap)
	{
		pScene.second->Initialize();
	}
}

void empire::SceneManager::Update()
{
	GetActiveScene()->Update();
}

void empire::SceneManager::Render()
{
	if (m_pActiveScene == nullptr)
	{
		Debugger::GetInstance().LogError("SceneManager::Render - > no scene active");
	}
	m_pActiveScene->Render();
}

void SceneManager::LoadScene(std::string const& name)
{
	auto scene = m_pScenesMap.at(name);
	if (scene == nullptr)
	{
		Debugger::GetInstance().LogWarning("SceneManager::LoadScene - > Wrong scene name!");
		return;
	}
	if (!scene->IsActive())
	{
		auto activeScene = GetActiveScene();
		activeScene->m_bIsActive = false;
		scene->m_bIsActive = true;
		m_pActiveScene = scene;
		m_pActiveScene->OnActivate();
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
	if (m_pActiveScene == nullptr)
	{
		Debugger::GetInstance().LogError("SceneManager::GetActiveScene - > no scene active");
		return nullptr;
	}
	return m_pActiveScene;
	
}

void SceneManager::AddScene(Scene* pScene)
{
	if(m_pScenesMap.find(pScene->m_Name) != m_pScenesMap.end())
	{
		Debugger::GetInstance().LogError("SceneManager::AddScene - > The scene " + pScene->m_Name + " can't be added twice");
		
	}
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
	if (m_pActiveScene != nullptr)
	{
		m_pActiveScene->m_bIsActive = false;
	}
	Renderer::GetInstance().SetBackgroundColor(0, 0, 0, 0);

	if (m_pScenesMap.find(name) == m_pScenesMap.end())
	{
		Debugger::GetInstance().LogError("No existing scene with the name " + name);
	}
	auto newScene = m_pScenesMap.at(name);
	newScene->m_bIsActive = true;
	m_pActiveScene = newScene;
	m_pActiveScene->OnActivate();
}