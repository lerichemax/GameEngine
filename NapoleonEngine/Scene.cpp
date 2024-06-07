#include "PCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "SceneRenderer.h"
#include "RendererComponent.h"
#include "TextRendererComponent.h"
#include "TransformComponent.h"
#include "AudioSystem.h"
#include "System.h"
#include "BehaviourSystem.h"

#include "PrefabsManager.h"

#include <algorithm>
#include <map>

#include "CameraComponent.h"
#include "Timer.h"

//************************************
// BaseScene
//************************************

BaseScene::BaseScene()
	: m_pRegistry(std::make_shared<Coordinator>()),
	m_Name{ },
	m_pObjects()
{
}

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

std::shared_ptr<GameObject> BaseScene::FindTagInChildren(std::shared_ptr<GameObject> pObj, std::string const& tag) const
{
	std::unordered_set<Entity> children = m_pRegistry->GetChildren(pObj->GetEntity());

	for (Entity entity : children)
	{
		if (m_pRegistry->HasTag(entity, tag))
		{
			return GetGameObjectWithEntity(entity);
		}
	}

	Debugger::GetInstance().LogWarning("No GameObject found with tag " + tag);
	return nullptr;
}

std::vector<std::shared_ptr<GameObject>> BaseScene::GetChildrenWithTag(std::shared_ptr<GameObject> pObj, std::string const& tag) const
{
	std::vector<std::shared_ptr<GameObject>> childrenWithTag;

	std::unordered_set<Entity> children = m_pRegistry->GetChildren(pObj->GetEntity());

	for (Entity entity : children)
	{
		std::shared_ptr<GameObject> pChild = GetGameObjectWithEntity(entity);

		if (pChild != nullptr && pChild->m_Tag == tag)
		{
			childrenWithTag.push_back(pChild);
		}
	}

	return childrenWithTag;
}

std::shared_ptr<GameObject> BaseScene::CreateGameObjectNoTransform()
{
	m_pObjects.push_back(std::shared_ptr<GameObject>(new GameObject{ m_pRegistry, false}));

	return m_pObjects.back();
}

std::shared_ptr<GameObject> BaseScene::GetGameObjectWithEntity(Entity entity) const
{
	for (auto& pObj : m_pObjects)
	{
		if (pObj->GetEntity() == entity)
		{
			return pObj;
		}
	}

	return nullptr;
}

//************************************
// Prefab
//************************************

Prefab::Prefab()
	:BaseScene(),
	m_pRootObject{ BaseScene::CreateGameObject() }
{
}

std::shared_ptr<GameObject> Prefab::CreateGameObject()
{
	auto pNewobject = BaseScene::CreateGameObject();

	m_pRootObject->AddChild(pNewobject);

	return pNewobject;
}

std::shared_ptr<GameObject> Prefab::GetRoot() const
{
	return m_pRootObject;
}


void Prefab::Serialize(StreamWriter& writer) const
{
	writer.StartObject("Prefab");
	writer.WriteString("name", m_Name);
	writer.StartArray("gameobjects");
	for (std::shared_ptr<GameObject> pObject : m_pObjects)
	{
		writer.StartArrayObject();
		pObject->Serialize(writer);
		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();
}

void Prefab::SetName(std::string const& name)
{
	m_Name = name;
}

//************************************
// Scene
//************************************

Scene::Scene(const std::string& name)
	: BaseScene(name),
	m_pColliders(),
	m_pSceneRenderer(new SceneRenderer{}),
	m_pTransformSystem(m_pRegistry->RegisterSystem<TransformSystem>()),
	m_pAudio(m_pRegistry->RegisterSystem<AudioSystem>()),
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

	AddSystem<BehaviourSystem>();
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

	m_pAudio->Update(m_pRegistry->GetComponentManager());
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

void Scene::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	auto objects = reader->ReadArray("gameobjects");

	for (SizeType i = 0; i < objects->GetArraySize(); i++)
	{
		auto pNewObject = CreateGameObject();
		pNewObject->Deserialize(objects->ReadArrayIndex(i).get(), context);
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

std::shared_ptr<GameObject> Scene::InstantiatePrefab(std::string const& name)
{
	PrefabsManager::GetInstance().InstantiatePrefab(name, this);
	return m_pObjects.back();
}

void Scene::AddCollider(ColliderComponent* pCollider)
{
	m_pColliders.emplace_back(pCollider);
}

void Scene::RemoveCollider(ColliderComponent* pCollider)
{
	m_pColliders.erase(std::remove(m_pColliders.begin(), m_pColliders.end(), pCollider));
}