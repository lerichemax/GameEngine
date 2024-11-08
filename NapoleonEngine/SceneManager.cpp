#include "PCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "NapoleonEngine.h"
#include "Timer.h"

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

	void Initialize();
	void Update();
	
	Scene const* GetScene(std::string const& sceneName) const;
	void AddScene(Scene* pScene);
	void RenameScene(std::string const& oldName, std::string const& newName);
	void LoadScene(std::string const& name);
	void ReloadCurrentScene();
	Scene* GetActiveScene() const;

private:
	Scene* m_pNextActiveScene;

	Scene* m_pActiveScene;
	std::map <std::string, Scene*> m_pScenesMap;
};

SceneManager::SceneManagerImpl::SceneManagerImpl()
	:m_pScenesMap(),
	m_pActiveScene(nullptr)
{
}

SceneManager::SceneManagerImpl::~SceneManagerImpl()
{
	for (auto pScene : m_pScenesMap)
	{
		SAFE_DELETE(pScene.second);
	}
}

void SceneManager::SceneManagerImpl::Update()
{
	if (m_pNextActiveScene != nullptr)
	{
		if (m_pActiveScene != nullptr)
		{
			m_pActiveScene->CleanUpScene();
		}
		
		m_pActiveScene = m_pNextActiveScene;
		m_pActiveScene->m_bIsActive = true;
		m_pActiveScene->OnLoad();
		
		m_pNextActiveScene = nullptr;
	}

	if (m_pActiveScene != nullptr)
	{
		m_pActiveScene->Update();
	}
	else
	{
		NapoleonEngine::Quit();
	}
}

Scene const* SceneManager::SceneManagerImpl::GetScene(std::string const& sceneName) const
{
	try
	{
		return m_pScenesMap.at(sceneName);
	}
	catch( std::out_of_range const& error)
	{
		LOG_ERROR(error.what());
	}
	return nullptr;
}

void SceneManager::SceneManagerImpl::LoadScene(std::string const& name)
{
	auto scene = m_pScenesMap.at(name);
	if (scene == nullptr)
	{
		LOG_WARNING("SceneManager::LoadScene - > Wrong scene name!");
		return;
	}
	
	m_pNextActiveScene = scene;
}

void SceneManager::SceneManagerImpl::ReloadCurrentScene()
{
	LoadScene(GetActiveScene()->m_Name);
}

Scene* SceneManager::SceneManagerImpl::GetActiveScene() const
{
	if (m_pActiveScene == nullptr)
	{
		LOG_ERROR("SceneManager::Render - > no scene active");
		NapoleonEngine::Quit();
		return nullptr;
	}

	return m_pActiveScene;
}

void SceneManager::SceneManagerImpl::AddScene(Scene* pScene)
{
	if (m_pScenesMap.find(pScene->m_Name) != m_pScenesMap.end())
	{
		LOG_ERROR("SceneManager::AddScene - > The scene %s can't be added twice", pScene->GetName().c_str());

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

SceneManager::SceneManager()
	:Singleton<SceneManager>(),
	m_pImpl(new SceneManagerImpl())
{
	
}

SceneManager::~SceneManager()
{

}

void SceneManager::Update()
{
	m_pImpl->Update();
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