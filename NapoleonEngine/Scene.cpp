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
#include "UiSystem.h"

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
	m_pSystems.clear();
}

std::shared_ptr<GameObject> BaseScene::CreateGameObject()
{
	auto newObject = std::shared_ptr<GameObject>(new GameObject{ m_pRegistry }) ;
	
	ECS_TransformComponent transformComp;
	newObject->AddComponent<ECS_TransformComponent>(transformComp);

	m_pObjects.push_back(newObject);

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

		if (pChild != nullptr && pChild->GetTag() == tag)
		{
			childrenWithTag.push_back(pChild);
		}
	}

	return childrenWithTag;
}

void BaseScene::Serialize(StreamWriter& writer) const
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
	writer.StartArray("systems");
	for (std::shared_ptr<System> pSystem : m_pSystems)
	{
		writer.WriteStringNoKey(pSystem->GetNameForSerialization());
	}
	writer.EndArray();
	writer.EndObject();
}

void BaseScene::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	auto prefab = reader->ReadObject("Prefab");
	//prefab->ReadString("name", m_Name);

	auto systems = prefab->ReadArray("systems");
	for (SizeType i = 0; i < systems->GetArraySize(); i++)
	{
		auto pNewSystem = m_pRegistry->AddSystemFromName(systems->ReadArrayIndexAsString(i));
		if (pNewSystem != nullptr)
		{
			m_pSystems.push_back(pNewSystem);
		}
	}

	auto objects = prefab->ReadArray("gameobjects");

	for (SizeType i = 0; i < objects->GetArraySize(); i++)
	{
		auto pNewObject = CreateGameObjectNoTransform();
		int entity;
		auto go = objects->ReadArrayIndex(i);
		go->ReadInt("Entity", entity);
		context.Add(entity, pNewObject);
		pNewObject->Deserialize(objects->ReadArrayIndex(i).get(), context);
	}
}

void BaseScene::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	auto prefab = reader->ReadObject("Prefab");
	auto objects = prefab->ReadArray("gameobjects");

	size_t arraySize = objects->GetArraySize();

	for (size_t i = 0; i < arraySize; i++)
	{
		m_pObjects[m_pObjects.size() - (arraySize - i)]->RestoreContext(objects->ReadArrayIndex(i).get(), context);
	}
}

std::shared_ptr<GameObject> BaseScene::CreateGameObjectNoTransform()
{
	m_pObjects.push_back(std::shared_ptr<GameObject>(new GameObject{ m_pRegistry}));

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

bool Prefab::IsRoot(std::shared_ptr<GameObject> pObject) const
{
	return pObject->GetEntity() == m_pRootObject->GetEntity();
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
	m_pBehaviours(m_pRegistry->RegisterSystem<BehaviourSystem>()),
	m_pUi(m_pRegistry->RegisterSystem<UiSystem>()),
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
	DeclareInput();
	Initialize();
}

void Scene::Update()
{	
	for (auto& pSystem : m_pSystems)
	{
		pSystem->Update(m_pRegistry->GetComponentManager());
	}

	m_pBehaviours->Update(m_pRegistry->GetComponentManager());
	m_pTransformSystem->Update(m_pRegistry->GetComponentManager());
	m_pUi->Update(m_pRegistry->GetComponentManager());

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
	size_t index = m_pObjects.size();

	PrefabsManager::GetInstance().InstantiatePrefab(name, this);

	if (m_pObjects.size() > index)
	{
		for (Entity child : m_pRegistry->GetEntityHierarchy(m_pObjects[index]->m_Entity))
		{
			m_pBehaviours->Initialize(child, m_pRegistry->GetComponentManager());
		}

		return m_pObjects[index];
	}

	return nullptr;
}

std::shared_ptr<GameObject> Scene::InstantiatePrefab(std::string const& name, glm::vec2 const& location)
{
	size_t index = m_pObjects.size();

	PrefabsManager::GetInstance().InstantiatePrefab(name, this);

	if (m_pObjects.size() > index)
	{
		m_pObjects[index]->GetTransform()->Translate(location);
		for (Entity child : m_pRegistry->GetEntityHierarchy(m_pObjects[index]->m_Entity))
		{
			m_pBehaviours->Initialize(child, m_pRegistry->GetComponentManager());
		}

		return m_pObjects[index];
	}

	return nullptr;
}

void Scene::AddCollider(ColliderComponent* pCollider)
{
	m_pColliders.emplace_back(pCollider);
}

void Scene::RemoveCollider(ColliderComponent* pCollider)
{
	m_pColliders.erase(std::remove(m_pColliders.begin(), m_pColliders.end(), pCollider));
}