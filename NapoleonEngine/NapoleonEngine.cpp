#include "PCH.h"
#include "NapoleonEngine.h"

#include <SDL_mixer.h>
#include <functional>

#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SoundServiceLocator.h"
#include "Timer.h"
#include "PrefabsManager.h"

#include "TextRendererComponent.h"
#include "TransformComponent.h"
#include "FPSCounter.h"

using namespace std;
bool NapoleonEngine::m_bQuit = false;
void empire::NapoleonEngine::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_WindowWidth = 900;
	m_WindowHeight = 600;
	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_WindowWidth,
		m_WindowHeight,
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

void NapoleonEngine::Quit()
{
	m_bQuit = true;
}


void empire::NapoleonEngine::Cleanup()
{
	auto pService = &SoundServiceLocator::GetService();
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
	SceneManager::GetInstance().Initialize(this);
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
			/*auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(m_MsPerFrame) - high_resolution_clock::now());
			this_thread::sleep_for(sleepTime);*/
		}
	}

	Cleanup();
}
