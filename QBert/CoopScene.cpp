#include "PCH.h"
#include "CoopScene.h"

#include "PrefabsManager.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"

using namespace empire;
CoopScene::CoopScene() : Scene("CoopScene")
{
	
}

void CoopScene::Initialize()
{
	AddObject(empire::PrefabsManager::GetInstance().Instantiate("FPSCounter"));

	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 15);
	
	auto const livesP1 = new GameObject;
	livesP1->AddComponent(new TextRendererComponent{ "P1 Lives:", font });
	livesP1->GetTransform()->Translate(20.f, 40.f);
	livesP1->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	AddObject(livesP1);

	auto const pointsP1 = new GameObject;
	pointsP1->AddComponent(new TextRendererComponent{ "P1 Points: 0 " , font });
	pointsP1->GetTransform()->Translate(20.f, 60.f);
	pointsP1->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	AddObject(pointsP1);

	auto const livesP2 = new GameObject;
	livesP2->AddComponent(new TextRendererComponent{ "P2 Lives: ", font });
	livesP2->GetTransform()->Translate(20.f, 80.f);
	livesP2->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 255);
	AddObject(livesP2);

	auto const pointsP2 = new GameObject{};
	pointsP2->AddComponent(new TextRendererComponent{ "P2 Points: 0 " , font });
	pointsP2->GetTransform()->Translate(20.f, 100.f);
	pointsP2->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 255);
	AddObject(pointsP2);

	
}