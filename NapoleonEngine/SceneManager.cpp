#include "PCH.h"
#include "SceneManager.h"
#include "Scene.h"

#include <map>

class SceneManager::SceneManagerImpl final
{
public:
	SceneManagerImpl();
	SceneManagerImpl(SceneManagerImpl const& other) = delete;
	SceneManagerImpl(SceneManagerImpl&& other) = delete;
	SceneManagerImpl& operator=(SceneManagerImpl const& rhs) = delete;
	SceneManagerImpl& operator=(SceneManagerImpl&& rhs) = delete;
	~SceneManagerImpl();

	void Initialize(NapoleonEngine const* pEngine);
	void Update();
	void Render();
	
	Scene const* GetScene(std::string const& sceneName) const;
	void AddScene(Scene* pScene);
	void RenameScene(std::string const& oldName, std::string const& newName);
	void SetSceneActive(std::string const& name);
	void LoadScene(std::string const& name);
	void ReloadCurrentScene();
	Scene* GetActiveScene() const;
	NapoleonEngine const* GetEngine()const { return m_pEngine; }

private:
	Scene* m_pNextActiveScene;

	Scene* m_pActiveScene;
	NapoleonEngine const* m_pEngine;
	std::map <std::string, Scene*> m_pScenesMap;
};

SceneManager::SceneManagerImpl::SceneManagerImpl()
	:m_pScenesMap(),
	m_pEngine(nullptr),
	m_pActiveScene(nullptr),
	m_pNextActiveScene(nullptr)
{
}

SceneManager::SceneManagerImpl::~SceneManagerImpl()
{
	for (auto pScene : m_pScenesMap)
	{
		SafeDelete(pScene.second);
	}
}

void SceneManager::SceneManagerImpl::Initialize(NapoleonEngine const* pEngine)
{
	m_pEngine = pEngine;
	for (auto pScene : m_pScenesMap)
	{
		pScene.second->Initialize();
		pScene.second->DeclareInput();
	}
}

void SceneManager::SceneManagerImpl::Update()
{
	if (m_pNextActiveScene != nullptr)
	{
		m_pActiveScene = m_pNextActiveScene;
		m_pActiveScene->OnActivate();
		m_pNextActiveScene = nullptr;
	}
	m_pActiveScene->Update();
}

void SceneManager::SceneManagerImpl::Render()
{
	if (m_pActiveScene == nullptr)
	{
		Debugger::GetInstance().LogError("SceneManager::Render - > no scene active");
	}

	m_pActiveScene->Render();
}

Scene const* SceneManager::SceneManagerImpl::GetScene(std::string const& sceneName) const
{
	try
	{
		return m_pScenesMap.at(sceneName);
	}
	catch( std::out_of_range const& error)
	{
		Debugger::GetInstance().LogError(error.what());
	}
	return nullptr;
}

void SceneManager::SceneManagerImpl::LoadScene(std::string const& name)
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

void SceneManager::SceneManagerImpl::ReloadCurrentScene()
{
	LoadScene(GetActiveScene()->m_Name);
}

Scene* SceneManager::SceneManagerImpl::GetActiveScene() const
{
	if (m_pActiveScene == nullptr)
	{
		Debugger::GetInstance().LogError("SceneManager::GetActiveScene - > no scene active");
		return nullptr;
	}
	return m_pActiveScene;
}

void SceneManager::SceneManagerImpl::AddScene(Scene* pScene)
{
	if (m_pScenesMap.find(pScene->m_Name) != m_pScenesMap.end())
	{
		Debugger::GetInstance().LogError("SceneManager::AddScene - > The scene " + pScene->m_Name + " can't be added twice");

	}
	m_pScenesMap.insert(std::make_pair(pScene->m_Name, pScene));
}

void SceneManager::SceneManagerImpl::RenameScene(std::string const& oldName, std::string const& newName)
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

void SceneManager::SceneManagerImpl::SetSceneActive(std::string const& name)
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
	m_pNextActiveScene = newScene;
}

SceneManager::SceneManager()
	:Singleton<SceneManager>(),
	m_pImpl(new SceneManagerImpl())
{
	
}

SceneManager::~SceneManager()
{

}

void SceneManager::Initialize(NapoleonEngine const* pEngine)
{
	m_pImpl->Initialize(pEngine);
}

void SceneManager::Update()
{
	m_pImpl->Update();
}

void SceneManager::Render()
{
	m_pImpl->Render();
}

void SceneManager::LoadScene(std::string const& name)
{
	m_pImpl->LoadScene(name);
}

void SceneManager::ReloadCurrentScene()
{
	LoadScene(GetActiveScene()->m_Name);
}

Scene* SceneManager::GetActiveScene() const
{
	return m_pImpl->GetActiveScene();
}

void SceneManager::AddScene(Scene* pScene)
{
	m_pImpl->AddScene(pScene);
}

void SceneManager::RenameScene(std::string const& oldName, std::string const& newName)
{
	m_pImpl->RenameScene(oldName, newName);
}

void SceneManager::SetSceneActive(std::string const& name)
{
	m_pImpl->SetSceneActive(name);
}

NapoleonEngine const* SceneManager::GetEngine()const
{
	return m_pImpl->GetEngine();
}