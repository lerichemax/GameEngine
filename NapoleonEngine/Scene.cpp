#include "PCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "SceneRenderer.h"
#include "RendererComponent.h"
#include "TextRendererComponent.h"
#include "TransformComponent.h"
#include "System.h"

#include "PrefabsManager.h"

#include <algorithm>
#include <map>

#include "CameraComponent.h"
#include "Timer.h"

BaseScene::BaseScene(const std::string& name)
	: m_pRegistry(std::make_shared<Coordinator>()),
	m_Name{ name },
	m_pObjects()
{
}

BaseScene::~BaseScene()
{
	m_pObjects.clear();
	m_Systems.clear();
}

std::shared_ptr<GameObject> BaseScene::CreateGameObject()
{
	m_pObjects.push_back(std::shared_ptr<GameObject>(new GameObject{ m_pRegistry }));

	return m_pObjects.back();
}

std::shared_ptr<GameObject> BaseScene::InstantiatePrefab(std::string const& name)
{
	std::shared_ptr<Prefab> prefab = PrefabsManager::GetInstance().GetPrefab(name);

	if (prefab == nullptr)
	{
		Debugger::GetInstance().LogWarning("No prefab with name " + name + " found");
		return nullptr;
	}

	std::vector<std::shared_ptr<System>> prefabSystems = m_pRegistry->ExtractSystems(prefab->m_pRegistry);
	m_Systems.insert(m_Systems.end(), prefabSystems.begin(), prefabSystems.end());

	bool first = true;
	std::shared_ptr<GameObject> toReturn;

	std::map<Entity, Entity> processedEntities{};//key : prefab object entity //value : new object entity

	for (std::shared_ptr<GameObject> const pObject : prefab->m_pObjects)
	{
		std::shared_ptr<GameObject> pNewObject;

		if (processedEntities.find(pObject->m_Entity) == processedEntities.end())
		{
			pNewObject = CreateGameObject();
			processedEntities.insert(std::make_pair(pObject->m_Entity, pNewObject->m_Entity));

			if (first)
			{
				toReturn = pNewObject;
				first = false;
			}

			m_pRegistry->TransferComponents(pObject->m_Entity, pNewObject->m_Entity, prefab->m_pRegistry);
		}
		else {
			pNewObject = *(std::find_if(m_pObjects.begin(), m_pObjects.end(), [pObject](auto& pObj) {
				return pObj->m_Entity == pObject->m_Entity;
				}));
		}
		
		auto const& children = prefab->m_pRegistry->GetChildren(pObject->m_Entity);

		for (Entity entity : children)
		{
			if (processedEntities.find(entity) != processedEntities.end())
			{
				m_pRegistry->AddChild(pNewObject->m_Entity, processedEntities.at(entity));
			}
			else
			{
				std::shared_ptr<GameObject> pChildObject = CreateGameObject();
				processedEntities.insert(std::make_pair(entity, pChildObject->m_Entity));

				pNewObject->AddChild(pChildObject);

				m_pRegistry->TransferComponents(entity, pChildObject->m_Entity, prefab->m_pRegistry);
			}
			
		}
	}

	return toReturn;
}

Scene::Scene(const std::string& name)
	: BaseScene(name),
	m_pColliders(),
	m_pSceneRenderer(new SceneRenderer{}),
	m_pTransformSystem(m_pRegistry->RegisterSystem<TransformSystem>()),
	m_pTextRenderer(m_pRegistry->RegisterSystem<TextRendererSystem>()),
	m_pECS_SceneRenderer(m_pRegistry->RegisterSystem<LayeredRendererSystem>()),
	m_pActiveCamera(nullptr),
	m_pCamera(m_pRegistry->RegisterSystem<CameraSystem>()),
	m_bIsActive(false),
	m_bIsInitialized(false)
{
	m_pCameraObject = CreateGameObject();

	ECS_CameraComponent camComp{};
	m_pCameraObject->AddComponent<ECS_CameraComponent>(camComp);
	SetActiveCamera(m_pCameraObject);
}

Scene::~Scene()
{
	CleanUpScene();
}

void Scene::CleanUpScene()
{
	SafeDelete(m_pSceneRenderer);
}

void Scene::OnActivate()
{
	Timer::GetInstance().SetTimeScale(1);
	m_bIsActive = true;
	CustomOnActivate();
}

void Scene::Update()
{
	//To delete
	for(auto& object : m_pObjects)
	{
		if (object->IsActive())
		{
			object->Update();
		}
	}
	
	for (auto& pSystem : m_Systems)
	{
		pSystem->Update(m_pRegistry->GetComponentManager());
	}

	m_pTransformSystem->Update(m_pRegistry->GetComponentManager());

	CheckCollidersCollision();
	
	Refresh();
}

void Scene::Render() const
{
	if (!EntityManager::IsEntityValid(m_pCamera->m_MainCameraEntity))
	{
		Debugger::GetInstance().LogError("Scene::Render - > no camera currently active");
	}
	glPushMatrix();
	{
		m_pCamera->Update(m_pRegistry->GetComponentManager());
		m_pTextRenderer->Update(m_pRegistry->GetComponentManager());
		m_pECS_SceneRenderer->Update(m_pRegistry->GetComponentManager());
	}
	glPopMatrix();
}

void Scene::Refresh()
{
	m_pObjects.erase(std::remove_if(m_pObjects.begin(), m_pObjects.end(), [](std::shared_ptr<GameObject> pGo)
		{
			return pGo->m_bIsDestroyed;
		}),m_pObjects.end());
}

void Scene::AddToGroup(RendererComponent* pRenderer, Layer layer) const
{
	m_pSceneRenderer->AddToGroup(pRenderer, layer);
}

void Scene::RemoveFromGroup(RendererComponent* pRenderer, Layer layer) const
{
	m_pSceneRenderer->RemoveFromGroup(pRenderer, layer);
}

void Scene::CheckCollidersCollision()
{
	for(auto pColl : m_pColliders)
	{
		if (!pColl->GetGameObject()->IsActive())
		{
			continue;
		}
		for(auto pOtherColl : m_pColliders)
		{
			if (!pOtherColl->GetGameObject()->IsActive())
			{
				continue;
			}
			if (pColl == pOtherColl)
			{
				continue;
			}
			pColl->CheckOverlap(pOtherColl);
		}
	}
}

void Scene::SetActiveCamera(std::shared_ptr<GameObject> pGameObject)
{
	if (m_pCamera->TrySetMainCamera(pGameObject))
	{
		m_pCameraObject = pGameObject;
	}
}

std::shared_ptr<GameObject> Scene::GetCameraObject() const
{
	return m_pCameraObject;
}

void Scene::AddCollider(ColliderComponent* pCollider)
{
	m_pColliders.emplace_back(pCollider);
}

void Scene::RemoveCollider(ColliderComponent* pCollider)
{
	m_pColliders.erase(std::remove(m_pColliders.begin(), m_pColliders.end(), pCollider));
}