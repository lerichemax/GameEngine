#include "MiniginPCH.h"
#include "Minigin.h"

#include <chrono>
#include <thread>
#include <SDL.h>

#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Scene.h"
#include "RendererComponent.h"
#include "TextRendererComponent.h"
#include "SubjectComponent.h"
#include "Command.h"
#include "PlayerDieCommand.h"
#include "QBert.h"

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
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo", true);
	
	auto* go = new GameObject{Tag::other};
	go->AddComponent<RendererComponent>("background.jpg");
	scene.Add(go);

	go = new GameObject{Tag::other };
	go->AddComponent<RendererComponent>("logo.png");
	go->AddComponent<TransformComponent>(216.f, 180.f);
	scene.Add(go);
	
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = new GameObject{ Tag::other };
	to->AddComponent<TextRendererComponent>("Programming 4 Assignment", font);
	to->AddComponent<TransformComponent>(80.f, 20.f);
	scene.Add(to);
	
	auto const ui = new UIObject{};
	scene.Add(ui);
	
	auto const player = new QBert{ui};
	scene.Add(player);
	InputManager::GetInstance().AddCommand(SDLK_q, new PlayerDieCommand{ KeyActionState::pressed, player });
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	
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
			float const deltaTime = duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;
			doContinue = input.ProcessInput();

			sceneManager.Update(deltaTime);
			
			////Fps = 1/deltatime
			//while (lag >= m_MsPerFrame/1000.f) //wrong update loop (only physics)
			//{
			//	
			//	
			//	lag -= (m_MsPerFrame / 1000.f);
			//}
			////renderer.Render(lag / m_MsPerUpdate);
			
			renderer.Render();
			
			auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(m_MsPerFrame) - high_resolution_clock::now());
			this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}
