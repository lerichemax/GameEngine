#include "PCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include <algorithm>

using namespace empire;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name{name}
{
}

Scene::~Scene()
{
	CleanUpScene();
}

void Scene::CleanUpScene()
{
	for (auto pObject : m_pObjects)
	{
		delete pObject;
	}
	m_pObjects.clear();
}

void Scene::Add(GameObject* object)
{
	m_pObjects.emplace_back(std::move(object));
	object->m_pScene = this;
	for (auto pChild : object->m_pChildren)
	{
		pChild->m_pScene = this;
	}
}

void Scene::Update()
{
	for(auto& object : m_pObjects)
	{
		object->Update();
	}
	Refresh();
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

void Scene::Refresh()
{
	std::for_each(m_pObjects.begin(), m_pObjects.end(), [](GameObject* pObj)
		{
			pObj->Refresh();
		});
	
	m_pObjects.erase(std::remove_if(m_pObjects.begin(), m_pObjects.end(), [](GameObject* pOb)
		{
			return !pOb->IsActive();
		}),m_pObjects.end());
}