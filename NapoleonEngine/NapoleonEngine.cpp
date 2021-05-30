#include "PCH.h"
#include "NapoleonEngine.h"

#include <SDL_mixer.h>
#include <functional>

#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "ServiceLocator.h"
#include "Timer.h"
#include "PrefabsManager.h"

#include "TextRendererComponent.h"
#include "TransformComponent.h"
#include "FPSCounter.h"

using namespace std;

void empire::NapoleonEngine::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		900,
		600,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::string errorMsg{ "Core::Initialize( ), error when calling Mix_OpenAudio: " };
		errorMsg += Mix_GetError ();
		Debugger::GetInstance().LogError(errorMsg);
		return;
	}
	
	Renderer::GetInstance().Init(m_Window);
	
	std::srand(unsigned int(time(nullptr)));

	auto fpsCounter = new GameObject{};
	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 15);
	fpsCounter->AddComponent(new TextRendererComponent("FPS ", font));
	fpsCounter->AddComponent(new FPSCounter{});
	fpsCounter->GetTransform()->Translate(20.f, 20.f);
	PrefabsManager::GetInstance().AddPrefab("FPSCounter", fpsCounter);
}

void empire::NapoleonEngine::Cleanup()
{
	auto pService = &ServiceLocator::GetService();
	if (typeid(*pService) != typeid(NullSoundInterface))
	{
		delete pService;
	}
	pService = nullptr;
	Renderer::GetInstance().Destroy();
	
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void empire::NapoleonEngine::Run()
{
	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("Data/");

	Initialize();

	CreatePrefabs();
	LoadGame();
	SceneManager::GetInstance().Initialize();
	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		bool doContinue = true;
	
		while (doContinue)
		{
			Timer::GetInstance().Update();
			
			doContinue = input.ProcessInput();
			
			sceneManager.Update();	
			ServiceLocator::GetService().Update();
			
			renderer.Render();

			Timer::GetInstance().Sleep();
			/*auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(m_MsPerFrame) - high_resolution_clock::now());
			this_thread::sleep_for(sleepTime);*/
		}
	}

	Cleanup();
}
