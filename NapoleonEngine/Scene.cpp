#include "PCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextRendererSystem.h"
#include "TransformSystem.h"
#include "LayeredRendererSystem.h"
#include "CollisionSystem.h"
#include "AudioSystem.h"
#include "System.h"
#include "UiSystem.h"

#include "RendererComponent.h"
#include "TextRendererComponent.h"
#include "TransformComponent.h"


#include "PrefabsManager.h"

#include <algorithm>
#include <map>

#include "CameraComponent.h"
#include "Timer.h"

//************************************
// BaseScene
//************************************

BaseScene::BaseScene()
	: m_pRegistry(std::make_unique<Registry>()),
	m_Name{ }
{
}

BaseScene::BaseScene(const std::string& name)
	: m_pRegistry(std::make_unique<Registry>()),
	m_Name{ name }
{
}

BaseScene::~BaseScene()
{
	m_pRegistry.reset();
}

GameObject* const BaseScene::CreateGameObject()
{
	auto newObject = std::unique_ptr<GameObject>(new GameObject{ m_pRegistry->CreateEntity(), m_pRegistry.get()});
	
	newObject->AddComponent<TransformComponent>();

	return newObject.get();
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

	Debugger::Get().LogWarning("No GameObject found with tag " + tag);
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
	writer.WriteString("name", m_Name);

	m_pRegistry->SerializeEntities(writer);
}

void BaseScene::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	m_pRegistry->DeserializeEntities(reader, context);
}

void BaseScene::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	m_pRegistry->RestoreEntitiesContext(reader, context);
}

GameObject* const BaseScene::CreateGameObjectNoTransform()
{
	return new GameObject{ m_pRegistry->CreateEntity(), m_pRegistry.get() };
}

GameObject* const BaseScene::GetGameObjectWithEntity(Entity entity) const
{
	//for (auto& pObj : m_pObjects)
	//{
	//	if (pObj->GetEntity() == entity)
	//	{
	//		return pObj.get();
	//	}
	//}

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

void Prefab::Serialize(StreamWriter& writer) const
{
	BaseScene::Serialize(writer);

	writer.StartArray("systems");
	{
		for (size_t system : m_RequiredSystems)
		{
			writer.WriteIntNoKey(static_cast<int64>(system));
		}
	}
	writer.EndArray();
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
	m_pCollisionSystem(nullptr),
	m_pAudio(nullptr),
	m_pTextRenderer(nullptr),
	m_pLayeredRenderer(nullptr),
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
	m_pSystems.clear();
}

void Scene::OnLoad()
{
	Timer::Get().SetTimeScale(1);
	m_pRegistry = std::make_unique<Registry>();
	m_pTransformSystem = m_pRegistry->RegisterSystem<TransformSystem>();
	m_pCollisionSystem = m_pRegistry->RegisterSystem<CollisionSystem>();
	m_pAudio = m_pRegistry->RegisterSystem<AudioSystem>();
	m_pTextRenderer = m_pRegistry->RegisterSystem<TextRendererSystem>();
	m_pLayeredRenderer = m_pRegistry->RegisterSystem<LayeredRendererSystem>();
	//m_pShapeRenderer = m_pRegistry->RegisterSystem<ShapeRenderer>();
	m_pUi = m_pRegistry->RegisterSystem<UiSystem>();
	m_pCamera = m_pRegistry->RegisterSystem<CameraSystem>();

	m_pCameraObject = CreateGameObject();

	m_pCameraObject->AddComponent<ECS_CameraComponent>();
	SetActiveCamera(m_pCameraObject);

	m_bIsActive = true;
	
	DeclareInput();
	Initialize();

	size_t nbrSystems = m_pSystems.size();
	size_t counter = 0;

	while (counter < nbrSystems)
	{
		m_pSystems[counter]->Initialize();
		counter++;
		nbrSystems = m_pSystems.size();
	}

	nbrSystems = m_pSystems.size();
	counter = 0;
	while (counter < nbrSystems)
	{
		m_pSystems[counter]->Start();
		counter++;
		nbrSystems = m_pSystems.size();
	}

	Renderer::Get().SetBackgroundColor(m_BackgroundColor);
}

void Scene::Update()
{	
	for (auto* const pSystem : m_pSystems )
	{
		pSystem->Update();
	}

	m_pTransformSystem->Update(); // move to update systems above ? 
	m_pCollisionSystem->Update();
	m_pUi->Update();

	m_pAudio->Update();

	Refresh();
}

void Scene::Render() const
{
	if (!EntityManager::IsEntityValid(m_pCamera->m_MainCameraEntity))
	{
		Debugger::Get().LogError("Scene::Render - > no camera currently active");
	}

	m_pTextRenderer->Update();

	glPushMatrix();
	{	
		m_pCamera->Update();
		m_pLayeredRenderer->Update();
	}
	glPopMatrix();
}

void Scene::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	auto systems = reader->ReadArray("systems");

	if (IS_VALID(systems))
	{
		for (SizeType i = 0; i < systems->GetArraySize(); i++)
		{
			System* const pAddedSystem = m_pRegistry->AddSystemFromHash(static_cast<size_t>(systems->ReadArrayIndexAsInt64(i)));
			if (pAddedSystem != nullptr)
			{
				m_pSystems.push_back(pAddedSystem);
			}
		}
	}

	BaseScene::Deserialize(reader, context);
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

GameObject Scene::InstantiatePrefab(std::string const& name)
{
	size_t index = m_pRegistry->GetLivingEntitiesCount();

	PrefabsManager::Get().InstantiatePrefab(name, this);

	return GameObject(m_pRegistry->GetEntityAtIndex(index), m_pRegistry.get());
}

GameObject Scene::InstantiatePrefab(std::string const& name, glm::vec2 const& location)
{
	size_t index = m_pRegistry->GetLivingEntitiesCount();

	PrefabsManager::Get().InstantiatePrefab(name, this);

	GameObject newObject{ m_pRegistry->GetEntityAtIndex(index), m_pRegistry.get() };

	newObject.GetTransform()->Translate(location);

	return newObject;
}

void Scene::AddCollider(ColliderComponent* pCollider)
{
	m_pColliders.emplace_back(pCollider);
}

void Scene::RemoveCollider(ColliderComponent* pCollider)
{
	m_pColliders.erase(std::remove(m_pColliders.begin(), m_pColliders.end(), pCollider));
}