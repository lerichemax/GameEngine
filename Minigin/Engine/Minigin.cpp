#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "RendererComponent.h"
#include "TextRendererComponent.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	
	Renderer::GetInstance().Init(m_Window);

	m_pFpsCounter = new GameObject{};
	auto const font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	m_pFpsCounter->AddComponent<TextRendererComponent>("FPS ",font );
	m_pFpsCounter->AddComponent<TransformComponent>(20.f, 20.f);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	
	auto* go = new GameObject{};
	go->AddComponent<RendererComponent>("background.jpg");
	scene.Add(go);

	go = new GameObject{ };
	go->AddComponent<RendererComponent>("logo.png");
	go->AddComponent<TransformComponent>(216.f, 180.f);
	scene.Add(go);

	
	auto const font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = new GameObject{  };
	to->AddComponent<TextRendererComponent>("Programming 4 Assignment", font);
	to->AddComponent<TransformComponent>(80.f, 20.f);
	scene.Add(to);
	scene.Add(m_pFpsCounter);
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	delete m_pFpsCounter;
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
	
	Initialize();

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();

		bool doContinue = true;
		auto lastTime{ high_resolution_clock::now() };
		float lag = 0.f;
		while (doContinue)
		{
			const auto currentTime = high_resolution_clock::now();
			float deltaTime = duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;
			doContinue = input.ProcessInput();
			
			while (lag >= m_MsPerFrame/1000.f)
			{
				sceneManager.Update();
				
				lag -= (m_MsPerFrame / 1000.f);
			}
			m_pFpsCounter->GetComponent<TextRendererComponent>().SetText("FPS " + std::to_string(1000 / (int)m_MsPerFrame));
			m_pFpsCounter->Update();
			//renderer.Render(lag / m_MsPerUpdate);
			renderer.Render();
			
			auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(m_MsPerFrame) - high_resolution_clock::now());
			this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}
