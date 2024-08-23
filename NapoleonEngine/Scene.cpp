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
#include "ShapeRenderer.h"

#include "PrefabsManager.h"

#include <algorithm>
#include <map>

#include "CameraComponent.h"
#include "Timer.h"

//************************************
// BaseScene
//************************************

BaseScene::BaseScene()
	: m_pRegistry(std::make_unique<Coordinator>()),
	m_Name{ },
	m_pObjects()
{
}

BaseScene::BaseScene(const std::string& name)
	: m_pRegistry(std::make_unique<Coordinator>()),
	m_Name{ name },
	m_pObjects()
{
}

BaseScene::~BaseScene()
{
	m_pObjects.clear();
}

GameObject* const BaseScene::CreateGameObject()
{
	auto newObject = std::unique_ptr<GameObject>(new GameObject{ m_pRegistry.get()});
	
	newObject->AddComponent<ECS_TransformComponent>();

	m_pObjects.push_back(std::move(newObject));

	return m_pObjects.back().get();
}

GameObject* const BaseScene::FindTagInChildren(GameObject* const pObj, std::string const& tag) const
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

std::vector<GameObject*> BaseScene::GetChildrenWithTag(GameObject* const pObj, std::string const& tag) const
{
	std::vector<GameObject*> childrenWithTag;

	std::unordered_set<Entity> children = m_pRegistry->GetChildren(pObj->GetEntity());

	for (Entity entity : children)
	{
		auto pChild = GetGameObjectWithEntity(entity);

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
	{
		for (auto& pObject : m_pObjects)
		{
			writer.StartArrayObject();
			pObject->Serialize(writer);
			writer.EndObject();
		}
	}
	writer.EndArray();

	writer.StartArray("systems");
	{
		for (auto* const pSystem : m_pSystems)
		{
			writer.StartArrayObject();
			pSystem->Serialize(writer);
			writer.EndObject();
		}
	}
	writer.EndArray();

	writer.EndObject();
}

void BaseScene::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	auto prefab = reader->ReadObject("Prefab");

	auto systems = prefab->ReadArray("systems");

	for (SizeType i = 0; i < systems->GetArraySize(); i++)
	{
		auto sys = systems->ReadArrayIndex(i);
		m_pSystems.push_back(m_pRegistry->DeserializeSystem(sys.get(), context));
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

GameObject* const BaseScene::CreateGameObjectNoTransform()
{
	m_pObjects.push_back(std::move(std::unique_ptr<GameObject>(new GameObject{ m_pRegistry.get()})));

	return m_pObjects.back().get();
}

GameObject* const BaseScene::GetGameObjectWithEntity(Entity entity) const
{
	for (auto& pObj : m_pObjects)
	{
		if (pObj->GetEntity() == entity)
		{
			return pObj.get();
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

GameObject* const Prefab::CreateGameObject()
{
	auto pNewobject = BaseScene::CreateGameObject();

	m_pRootObject->AddChild(pNewobject);

	return pNewobject;
}

GameObject* const Prefab::GetRoot() const
{
	return m_pRootObject;
}

bool Prefab::IsRoot(GameObject* const pObject) const
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
	m_pTransformSystem(nullptr),
	m_pAudio(nullptr),
	m_pTextRenderer(nullptr),
	m_pLayeredRenderer(nullptr),
	m_pBehaviours(nullptr),
	m_pUi(nullptr),
	m_pCamera(nullptr),
	m_bIsActive(false),
	m_bIsInitialized(false)
{
}

Scene::~Scene()
{
	CleanUpScene();
}

void Scene::CleanUpScene()
{
	m_pObjects.clear();
	m_pRegistry.reset();
}

void Scene::OnLoad()
{
	Timer::GetInstance().SetTimeScale(1);
	m_pRegistry = std::make_unique<Coordinator>();
	m_pTransformSystem = m_pRegistry->RegisterSystem<TransformSystem>();
	m_pAudio = m_pRegistry->RegisterSystem<AudioSystem>();
	m_pTextRenderer = m_pRegistry->RegisterSystem<TextRendererSystem>();
	m_pLayeredRenderer = m_pRegistry->RegisterSystem<LayeredRendererSystem>();
	//m_pShapeRenderer = m_pRegistry->RegisterSystem<ShapeRenderer>();
	m_pBehaviours = m_pRegistry->RegisterSystem<BehaviourSystem>();
	m_pUi = m_pRegistry->RegisterSystem<UiSystem>();
	m_pCamera = m_pRegistry->RegisterSystem<CameraSystem>();

	m_pCameraObject = CreateGameObject();

	m_pCameraObject->AddComponent<ECS_CameraComponent>();
	SetActiveCamera(m_pCameraObject);

	m_bIsActive = true;
	
	DeclareInput();
	Initialize();

	for (System* const pSystem : m_pSystems)
	{
		pSystem->Initialize(m_pRegistry->GetComponentManager());
	}

	for (System* const pSystem : m_pSystems)
	{
		pSystem->Start(m_pRegistry->GetComponentManager());
	}

	Renderer::GetInstance().SetBackgroundColor(m_BackgroundColor);
}

void Scene::Update()
{	
	//m_pBehaviours->Update(m_pRegistry->GetComponentManager());

	for (auto* const pSystem : m_pSystems )
	{
		pSystem->Update(m_pRegistry->GetComponentManager());
	}

	m_pTransformSystem->Update(m_pRegistry->GetComponentManager()); // move to update systems above ? 
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

	m_pTextRenderer->Update(m_pRegistry->GetComponentManager());

	glPushMatrix();
	{	
		m_pCamera->Update(m_pRegistry->GetComponentManager());
		m_pLayeredRenderer->Update(m_pRegistry->GetComponentManager());
		//m_pShapeRenderer->Update(m_pRegistry->GetComponentManager());
	}
	glPopMatrix();

	
}

void Scene::Refresh()
{
	m_pObjects.erase(std::remove_if(m_pObjects.begin(), m_pObjects.end(), [](std::unique_ptr<GameObject> const& pGo)
		{
			return pGo->m_bIsDestroyed;
		}),m_pObjects.end());
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

void Scene::SetActiveCamera(GameObject* const pGameObject)
{
	if (m_pCamera->TrySetMainCamera(pGameObject))
	{
		m_pCameraObject = pGameObject;
	}
}

GameObject* const Scene::GetCameraObject() const
{
	return m_pCameraObject;
}

GameObject* const Scene::InstantiatePrefab(std::string const& name)
{
	size_t index = m_pObjects.size();

	PrefabsManager::GetInstance().InstantiatePrefab(name, this);

	if (m_pObjects.size() > index)
	{
		for (Entity child : m_pRegistry->GetEntityHierarchy(m_pObjects[index]->m_Entity))
		{
			m_pBehaviours->Initialize(child, m_pRegistry->GetComponentManager());
		}

		return m_pObjects[index].get();
	}

	return nullptr;
}

GameObject* const Scene::InstantiatePrefab(std::string const& name, glm::vec2 const& location)
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

		return m_pObjects[index].get();
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