#include "PCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "SceneRenderer.h"

#include <algorithm>

using namespace empire;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name{name},
	m_pSceneRenderer(new SceneRenderer{})
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
		SafeDelete(pObject);
	}
	m_pObjects.clear();
	SafeDelete(m_pSceneRenderer);
}

void Scene::AddObject(GameObject* object)
{
	m_pObjects.emplace_back(std::move(object));
	object->m_pScene = this;
	for (auto pChild : object->m_pChildren)
	{
		pChild->m_pScene = this;
	}
	object->Initialize();
}

void Scene::Update()
{
	for(auto& object : m_pObjects)
	{
		object->Update();
	}
	CheckCollidersCollision();
	
	Refresh();
}

void Scene::Render() const
{
	m_pSceneRenderer->Render();

}

void Scene::Refresh()
{
	for (auto pGo : m_pObjects)
	{
		pGo->Refresh();
		if (!pGo->IsActive())
		{
			delete pGo;
			pGo = nullptr;
		}
	}

	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), nullptr),m_pObjects.end());
}

void Scene::AddToGroup(RendererComponent* pRenderer, Layer layer)
{
	m_pSceneRenderer->AddToGroup(pRenderer, layer);
}

void Scene::RemoveFromGroup(RendererComponent* pRenderer, Layer layer)
{
	m_pSceneRenderer->RemoveFromGroup(pRenderer, layer);
}

void Scene::CheckCollidersCollision()
{
	for(auto pColl : m_pColliders)
	{
		for(auto pOtherColl : m_pColliders)
		{
			if (pColl == pOtherColl)
			{
				continue;
			}
			pColl->CheckOverlap(pOtherColl);
		}
	}
}

void Scene::AddCollider(ColliderComponent* pCollider)
{
	m_pColliders.emplace_back(pCollider);
}

void Scene::RemoveCollider(ColliderComponent* pCollider)
{
	m_pColliders.erase(std::remove(m_pColliders.begin(), m_pColliders.end(), pCollider));
}