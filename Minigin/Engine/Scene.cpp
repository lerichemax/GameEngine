#include "MiniginPCH.h"
#include "Scene.h"
#include "RendererComponent.h"
#include "TextObject.h"
#include "ResourceManager.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name{name},
	m_pFpsCounter{}
{
	auto const font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	m_pFpsCounter = new TextObject{ "FPS ", font };
	m_pFpsCounter->SetPosition(20, 20);
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
	m_pObjects.push_back(object);
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_pObjects)
	{
		object->Update();
	}
	m_pFpsCounter->SetText("FPS " + std::to_string(1000 / (int)deltaTime));
	m_pFpsCounter->Update();
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		object->GetComponent<RendererComponent>()->Render(object->GetPosition());
	}
	m_pFpsCounter->GetComponent<RendererComponent>()->Render(m_pFpsCounter->GetPosition());
}