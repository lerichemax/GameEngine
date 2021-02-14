#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TextObject.h"

dae::SceneManager::~SceneManager()
{
	for (auto pScene : m_pScenes)
	{
		delete pScene;
	}
}

void dae::SceneManager::Update(float deltaTime)
{
	for(auto& scene : m_pScenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_pScenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto pScene = new Scene{ name };
	m_pScenes.push_back(pScene);
	return *pScene;
}
