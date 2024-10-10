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
#include "CameraSystem.h"

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

std::shared_ptr<GameObject> BaseScene::CreateGameObject()
{
	auto newObject = std::shared_ptr<GameObject>{ new GameObject{m_pRegistry->CreateEntity(), m_pRegistry.get()} };
	
	newObject->AddComponent<TransformComponent>();

	return newObject;
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

std::shared_ptr<GameObject> BaseScene::CreateGameObjectNoTransform()
{
	return std::shared_ptr<GameObject>{ new GameObject{ m_pRegistry->CreateEntity(), m_pRegistry.get() } };
}

//************************************
// Prefab
//************************************

Prefab::Prefab()
	:BaseScene(),
	m_pRootEntity{ BaseScene::CreateGameObject()->GetEntity()}
{
}

std::shared_ptr<GameObject> Prefab::CreateGameObject()
{
	auto newobject = BaseScene::CreateGameObject();

	m_pRegistry->AddChild(m_pRootEntity, newobject->GetEntity());

	return newobject;
}

std::shared_ptr<GameObject> Prefab::GetRoot() const
{
	return std::shared_ptr<GameObject>(new GameObject{m_pRootEntity, m_pRegistry.get()});
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
	m_pCameraSystem(nullptr),
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
	m_pCameraSystem = m_pRegistry->RegisterSystem<CameraSystem>();

	auto pCameraObject = CreateGameObject();

	m_CameraEntity = pCameraObject->GetEntity();

	pCameraObject->AddComponent<CameraComponent>();
	m_pCameraSystem->TrySetMainCamera(m_CameraEntity);

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

void InitializeCamera()
{

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
}

void Scene::Render() const
{
	if (!EntityManager::IsEntityValid(m_pCameraSystem->m_MainCameraEntity))
	{
		Debugger::Get().LogError("Scene::Render - > no camera currently active");
	}

	m_pTextRenderer->Update();

	glPushMatrix();
	{	
		m_pCameraSystem->Update();
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

std::shared_ptr<GameObject> Scene::GetCameraObject() const
{
	return std::shared_ptr<GameObject>(new GameObject{m_CameraEntity, m_pRegistry.get()});
}

std::shared_ptr<GameObject> Scene::InstantiatePrefab(std::string const& name)
{
	int index = m_pRegistry->GetLivingEntitiesCount();

	PrefabsManager::Get().InstantiatePrefab(name, this);

	return std::shared_ptr<GameObject>(new GameObject{ m_pRegistry->GetEntityAtIndex(index), m_pRegistry.get() });
}

std::shared_ptr<GameObject> Scene::InstantiatePrefab(std::string const& name, glm::vec2 const& location)
{
	int index = m_pRegistry->GetLivingEntitiesCount();

	PrefabsManager::Get().InstantiatePrefab(name, this);

	auto pNewObject = std::shared_ptr<GameObject>(new GameObject{ m_pRegistry->GetEntityAtIndex(index), m_pRegistry.get() });

	pNewObject->GetTransform()->Translate(location);

	return pNewObject;
}

void Scene::AddCollider(ColliderComponent* pCollider)
{
	m_pColliders.emplace_back(pCollider);
}

void Scene::RemoveCollider(ColliderComponent* pCollider)
{
	m_pColliders.erase(std::remove(m_pColliders.begin(), m_pColliders.end(), pCollider));
}