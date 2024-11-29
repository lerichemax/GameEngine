#include "PCH.h"
#include "Scene.h"
#include "GameObject.h"

#include "TextRendererSystem.h"
#include "TransformSystem.h"
#include "CollisionSystem.h"
#include "AudioSystem.h"
#include "System.h"
#include "UiSystem.h"
#include "ScriptingSystem.h"
#include "NapoleonEngine.h"

#include "TransformComponent.h"

#include "PrefabsManager.h"

#include <algorithm>
#include <map>

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

void BaseScene::Deserialize(JsonReader* const reader, SerializationMap& context)
{
	m_pRegistry->DeserializeEntities(reader, context);
}

void BaseScene::RestoreContext(JsonReader* const reader, SerializationMap const& context)
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
	m_pRootEntity{ NULL_ENTITY }
{
}

std::shared_ptr<GameObject> Prefab::CreateGameObject()
{
	auto newobject = BaseScene::CreateGameObject();

	if (m_pRootEntity == NULL_ENTITY)
	{
		m_pRootEntity = newobject->GetEntity();
	}
	else
	{
		m_pRegistry->AddChild(m_pRootEntity, newobject->GetEntity());
	}

	return newobject;
}

void Prefab::SetName(std::string const& name)
{
	m_Name = name;
}

void Prefab::Serialize(StreamWriter& writer) const
{
	writer.WriteString("name", m_Name);

	m_pRegistry->SerializeEntities(writer);
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
	m_pUi(nullptr),
	m_pCamera(std::make_unique<Camera2D>(NapoleonEngine::GetEngine()->GetWindowWidth(), NapoleonEngine::GetEngine()->GetWindowHeight())),
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
	TimerLocator::Get()->SetTimeScale(1);
	m_pRegistry = std::make_unique<Registry>();
	m_pTransformSystem = m_pRegistry->RegisterSystem<TransformSystem>();
	m_pCollisionSystem = m_pRegistry->RegisterSystem<CollisionSystem>();
	m_pAudio = m_pRegistry->RegisterSystem<AudioSystem>();
	m_pUi = m_pRegistry->RegisterSystem<UiSystem>();

	m_pSystems.push_back(m_pRegistry->RegisterSystem<ScriptingSystem>());
	m_pSystems.push_back(m_pRegistry->RegisterSystem<TextRendererSystem>());

	m_bIsActive = true;
	
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
}

void Scene::Update()
{	
	for (auto* const pSystem : m_pSystems )
	{
		pSystem->Update();
	}

	m_pCamera->Update();
	m_pTransformSystem->Update(); // move to update systems above ? 
	m_pCollisionSystem->Update();
	m_pUi->Update();

	m_pAudio->Update();
}


void Scene::Deserialize(JsonReader* const reader, SerializationMap& context)
{
	BaseScene::Deserialize(reader, context);
}

Color const& Scene::GetBackgroundColor() const
{
	return m_BackgroundColor;
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

	pNewObject->GetTransform()->SetLocation(location);

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