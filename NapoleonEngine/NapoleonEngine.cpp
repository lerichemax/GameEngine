#include "PCH.h"
#include "NapoleonEngine.h"

#include <SDL_mixer.h>
#include <functional>

#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "PrefabsManager.h"
#include "Renderer.h"
#include "Timer.h"
#include "PrefabsManager.h"
#include "SerializerServiceLocator.h"
#include "Serializer.h"
#include "Deserializer.h"

#include "TextRendererComponent.h"
#include "TransformComponent.h"
#include "RendererComponent.h"

#include "FPSCounterSystem.h"

#include "FPSCounter.h"
#include "Scene.h"

class ObserverManager;

bool NapoleonEngine::m_bQuit = false;
NapoleonEngine* NapoleonEngine::m_pEngine = nullptr;

NapoleonEngine::NapoleonEngine(unsigned int Width, unsigned int Height, std::string const& name)
	:m_pRenderer{std::make_unique<Renderer>(Width, Height, name)}
{
	assert(m_pEngine == nullptr && "Trying to instantiate more than one Engine instance");

	m_pEngine = this;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::string errorMsg{ "Core::Initialize( ), error when calling Mix_OpenAudio: " };
		errorMsg += Mix_GetError();
		LOG_ERROR(errorMsg.c_str());
		return;
	}

	std::srand(unsigned int(time(nullptr)));

	try
	{
		ResourceManager::Get().Init("./Data/", m_pRenderer.get());
	}
	catch (std::runtime_error const& error)
	{
		LOG_ERROR(error.what());
	}

	SerializerServiceLocator::RegisterSerializer(new Serializer{});
	SerializerServiceLocator::RegisterDeserializer(new Deserializer{});
}

void NapoleonEngine::CreateBasePrefabs() //TODO : save and load from JSON
{
	//Fps counter prefab
	auto fpsCounterPrefab = PrefabsManager::Get().CreatePrefab();
	auto fpsCounterObject = fpsCounterPrefab->GetRoot();
	auto const font = ResourceManager::Get().GetFont("Fonts/Lingua.otf", 15);

	auto txtRenderer = fpsCounterObject->AddComponent<TextRendererComponent>();
	txtRenderer->SetText("FPS");
	txtRenderer->SetFont(font);
	txtRenderer->SetTextColor(0, 255, 0);
	
	fpsCounterObject->AddComponent<FPSCounter>();
	fpsCounterObject->AddComponent<RendererComponent>()->Layer = 10;
	fpsCounterObject->GetTransform()->SetLocation(20.f, 20.f);

	fpsCounterPrefab->AddRequiredSystem<FPSCounterSystem>();

	PrefabsManager::Get().SavePrefab(fpsCounterPrefab, "FPSCounter");

	//game specific prefab
	CreatePrefabs();
}

void NapoleonEngine::Quit()
{
	m_bQuit = true;
}

NapoleonEngine* NapoleonEngine::GetEngine()
{
	return m_pEngine;
}

void NapoleonEngine::RegisterSingleton(SingletonWrapper* singleton)
{
	m_Singletons.push_back(singleton);
}

unsigned int NapoleonEngine::GetWindowWidth() const
{
	return m_pRenderer->GetWindowWidth();
}

unsigned int NapoleonEngine::GetWindowHeight() const
{
	return m_pRenderer->GetWindowHeight();
}

void NapoleonEngine::Cleanup()
{
	for (size_t i = 0; i < m_Singletons.size(); i++)
	{
		delete m_Singletons[i];
	}

	m_Singletons.clear();

	m_pEngine = nullptr;
	
	SDL_Quit();

	SerializerServiceLocator::CleanUp();
	ComponentManager::CleanUp();
}

void NapoleonEngine::Run()
{
	CreateBasePrefabs();
	InitGame();
	{
		auto& sceneManager = SceneManager::Get();
		auto& input = InputManager::Get();
	
		while (!m_bQuit)
		{
			Timer::Get().Update();
			
			m_bQuit = !input.ProcessInput();
			sceneManager.Update();
			
			m_pRenderer->Render(sceneManager.GetActiveScene()->m_pRegistry.get(), sceneManager.GetActiveScene()->GetBackgroundColor());

			Timer::Get().Sleep();
		}
	}

	Cleanup();
}
