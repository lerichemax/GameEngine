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
#include "Serializer.h"
#include "Deserializer.h"

#include "TextRendererComponent.h"
#include "TransformComponent.h"
#include "RendererComponent.h"
#include "ButtonComponent.h"
#include "ColliderComponent.h"
#include "ScriptComponent.h"

#include "FPSCounter.h"
#include "Scene.h"

bool NapoleonEngine::m_bQuit = false;
NapoleonEngine* NapoleonEngine::m_pEngine = nullptr;

NapoleonEngine::NapoleonEngine(unsigned int Width, unsigned int Height, std::string const& name, bool bCreatePrefabs)
	: m_pRenderer{std::make_unique<Renderer>(Width, Height, name)},
	m_pTimer{ std::unique_ptr<Timer>(new Timer{}) },
	m_bCreatePrefabs{ bCreatePrefabs }
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
		ResourceManager::Get().Init(m_pRenderer.get());
	}
	catch (std::runtime_error const& error)
	{
		LOG_ERROR(error.what());
	}

	TimerLocator::RegisterTimer(m_pTimer.get());
}

void NapoleonEngine::Quit()
{
	m_bQuit = true;
}

NapoleonEngine* NapoleonEngine::GetEngine()
{
	return m_pEngine;
}

int NapoleonEngine::GetWindowWidth() const
{
	return m_pRenderer->GetWindowWidth();
}

int NapoleonEngine::GetWindowHeight() const
{
	return m_pRenderer->GetWindowHeight();
}

void NapoleonEngine::Cleanup()
{
	m_pEngine = nullptr;
	
	SDL_Quit();

	ComponentManager::CleanUp();
}

void NapoleonEngine::Run()
{
	StartHeapControl();

	if (m_bCreatePrefabs)
	{
		CreatePrefabs();
	}

	InitGame();

	{
		auto& sceneManager = SceneManager::Get();
		auto& input = InputManager::Get();
	
		while (!m_bQuit)
		{
			m_pTimer->Update();
			
			m_bQuit = !input.ProcessInput();
			sceneManager.Update();
			
			m_pRenderer->Render(sceneManager.GetActiveScene()->m_pRegistry.get(), sceneManager.GetActiveScene()->GetBackgroundColor());

			m_pTimer->Sleep();
		}
	}

	Cleanup();

	DumpMemoryLeaks();
}

void NapoleonEngine::StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc(165);

#endif
}

void NapoleonEngine::DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}