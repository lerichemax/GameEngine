#include "PCH.h"
#include "NapoleonEngine.h"

#include <SDL_mixer.h>
#include <functional>

#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "ObserverManager.h"
#include "PrefabsManager.h"
#include "Renderer.h"
#include "SoundServiceLocator.h"
#include "Timer.h"
#include "PrefabsManager.h"

#include "TextRendererComponent.h"
#include "TransformComponent.h"

#include "FPSCounter.h"
#include "Scene.h"


class ObserverManager;

using namespace std;
bool NapoleonEngine::m_bQuit = false;
NapoleonEngine* NapoleonEngine::m_pEngine = nullptr;

NapoleonEngine::NapoleonEngine()
{
	assert(m_pEngine == nullptr && "Trying to instantiate more than one Engine instance");

	m_pEngine = this;
}

void NapoleonEngine::Initialize(unsigned int width, unsigned int height, std::string const& name)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::string errorMsg{ "Core::Initialize( ), error when calling Mix_OpenAudio: " };
		errorMsg += Mix_GetError ();
		Debugger::GetInstance().LogError(errorMsg);
		return;
	}
	
	Renderer::GetInstance().Init(width, height, name);
	
	std::srand(unsigned int(time(nullptr)));

	try
	{
		ResourceManager::GetInstance().Init("./Data/");
	}
	catch (std::runtime_error const& error)
	{
		Debugger::GetInstance().LogError(error.what());
	}
}

void NapoleonEngine::CreateBasePrefabs()
{
	//Fps counter prefab
	auto fpsCounterPrefab = PrefabsManager::GetInstance().CreatePrefab("FPSCounter");
	auto fpsCounterObject = fpsCounterPrefab->CreateGameObject();
	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 15);

	ECS_TextRendererComponent txtRenderer("FPS ", font);
	fpsCounterObject->AddComponent<ECS_TextRendererComponent>(txtRenderer, true);
	fpsCounterObject->GetECSTransform()->Translate(20.f, 20.f);

	fpsCounterPrefab->AddSystem<FPSCounterSystem>(); 

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

void NapoleonEngine::Cleanup()
{
	auto pService = &SoundServiceLocator::GetService();
	if (typeid(*pService) != typeid(NullSoundInterface))
	{
		delete pService;
	}
	pService = nullptr;

	for (size_t i = 0; i < m_Singletons.size(); i++)
	{
		delete m_Singletons[i];
	}

	m_Singletons.clear();

	m_pEngine = nullptr;
	
	SDL_Quit();
}

void NapoleonEngine::Run()
{
	CreateBasePrefabs();
	InitGame();
	SceneManager::GetInstance().Initialize();
	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
	
		while (!m_bQuit)
		{
			Timer::GetInstance().Update();
			
			m_bQuit = !input.ProcessInput();
			
			sceneManager.Update();	
			SoundServiceLocator::GetService().Update();
			
			renderer.Render();

			Timer::GetInstance().Sleep();
		}
	}

	Cleanup();
}
