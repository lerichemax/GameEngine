#include "PCH.h"
#include "FPSCounter.h"
#include "TextRendererComponent.h"
#include "ResourceManager.h"

empire::FPSCounter::FPSCounter()
{
	auto const font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	AddComponent(new TextRendererComponent("FPS ", font));
	GetTransform()->Translate(20.f, 20.f);
}

void empire::FPSCounter::Update(float deltaTime)
{
	GetComponent<TextRendererComponent>()->SetText("FPS " + std::to_string(int((1 / deltaTime))));
	GameObject::Update(deltaTime);
}