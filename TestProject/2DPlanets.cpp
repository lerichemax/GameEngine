#include "PCH.h"
#include "2DPlanets.h"

#include "GravitySystem.h"

#include "RendererComponent.h"
#include "RotationComponent.h"

Planets::Planets()
	:Scene("Planets")
{
}

void Planets::Initialize()
{
	auto pSun = CreateGameObject();
	auto pRenderer = pSun->AddComponent<RendererComponent>();

	pRenderer->SetShape(std::make_unique<Circle>(glm::vec2{ 0,0 }, 50, Color{ 255,0,0 }));

	auto pPlanet = CreateGameObject();
	auto pPlanetRenderer = pPlanet->AddComponent<RendererComponent>();

	pPlanetRenderer->SetShape(std::make_unique<Circle>(glm::vec2{ 0,0 }, 20, Color{ 0,255,0 }));

	auto pPlanetRotator = pPlanet->AddComponent<RotationComponent>();
	pPlanetRotator->Speed = 0.25f;

	pSun->AddChild(pPlanet);

	pPlanet->GetTransform()->SetLocalLocation({ 300, 0 });

	auto pMoon = CreateGameObject();
	auto pMoonRenderer = pMoon->AddComponent<RendererComponent>();

	pMoonRenderer->SetShape(std::make_unique<Circle>(glm::vec2{ 0,0 }, 10, Color{ 255,255,255 }));

	auto pMoonRotator = pMoon->AddComponent<RotationComponent>();
	pMoonRotator->Speed = 1;

	pPlanet->AddChild(pMoon);

	pMoon->GetTransform()->SetLocalLocation({ 100, 0 });

	AddSystem<GravitySystem>();
}