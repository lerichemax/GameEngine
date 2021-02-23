#include "MiniginPCH.h"
#include "Scene.h"
#include "RendererComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name, bool bIncludeFpsCounter)
	: m_Name{name},
	m_bHasFpsCounter(bIncludeFpsCounter)

{
	if (bIncludeFpsCounter)
	{
		AddFpsCounter();
	}
}

Scene::~Scene()
{
	for (auto pObject : m_pObjects)
	{
		delete pObject;
	}
	m_pObjects.clear();
}

void Scene::Add(SceneObject* object)
{
	m_pObjects.emplace_back(std::move(object));
}

void Scene::AddFpsCounter()
{
	m_pFpsCounter = new GameObject{};
	auto const font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	m_pFpsCounter->AddComponent<TextRendererComponent>("FPS ", font);
	m_pFpsCounter->AddComponent<TransformComponent>(20.f, 20.f);
}

void Scene::Update(float deltaTime)
{
	if (m_bHasFpsCounter)
	{
		m_pFpsCounter->GetComponent<TextRendererComponent>().SetText("FPS " + std::to_string(int((1 / deltaTime))));
		m_pFpsCounter->Update(deltaTime);
	}
	for(auto& object : m_pObjects)
	{
		object->Update(deltaTime);
	}
	Refresh();
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		if (object->HasComponent<RendererComponent>())
		{
			if (object->HasComponent<TransformComponent>())
			{
				object->GetComponent<RendererComponent>().Render(object->GetComponent<TransformComponent>());
			}
			else
			{
				object->GetComponent<RendererComponent>().Render();
			}
		}
		if (object->HasComponent<TextRendererComponent>())
		{
			if (object->HasComponent<TransformComponent>())
			{
				object->GetComponent<TextRendererComponent>().Render(object->GetComponent<TransformComponent>());
			}
			else
			{
				object->GetComponent<TextRendererComponent>().Render();
			}
		}
	}
	if (m_bHasFpsCounter)
	{
		m_pFpsCounter->GetComponent<TextRendererComponent>().Render(m_pFpsCounter->GetComponent<TransformComponent>());
	}
}

void Scene::Refresh()
{
	m_pObjects.erase(std::remove_if(m_pObjects.begin(), m_pObjects.end(), [](SceneObject* pOb)
		{
			return !pOb->IsActive();
		}),m_pObjects.end());
}