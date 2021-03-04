#include "MiniginPCH.h"
#include "MainGame.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"

#include "PlayerObserver.h"
#include "Scene.h"
#include "SceneObject.h"
#include "GameObject.h"
#include "QBert.h"
#include "UIObject.h"

#include "RendererComponent.h"
#include "TextRendererComponent.h"

#include "EarnPointsCommand.h"
using namespace dae;

void MainGame::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo", true);

	auto* go = new GameObject{ Tag::other };
	go->AddComponent<RendererComponent>("background.jpg");
	scene.Add(go);

	go = new GameObject{ Tag::other };
	go->AddComponent<RendererComponent>("logo.png");
	go->AddComponent<TransformComponent>(216.f, 180.f);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = new GameObject{ Tag::other };
	to->AddComponent<TextRendererComponent>("Programming 4 Assignment", font);
	to->AddComponent<TransformComponent>(80.f, 20.f);
	scene.Add(to);

	auto const pointsP1 = new UIObject{};
	pointsP1->GetComponent<TransformComponent>().SetPosition(20.f, 50.f, 0.f);
	pointsP1->GetComponent<TextRendererComponent>().SetText("Points: 0 ");
	scene.Add(pointsP1);

	auto const pointsP2 = new UIObject{};
	pointsP2->GetComponent<TransformComponent>().SetPosition(20.f, 70.f, 0.f);
	pointsP2->GetComponent<TextRendererComponent>().SetText("Points: 0 ");
	scene.Add(pointsP2);

	auto obs1 = new PlayerObserver{ pointsP1 };
	auto obs2 = new PlayerObserver{ pointsP2 };

	auto const player1 = new QBert{ Tag::player1 };
	player1->AddObserver(obs1);
	scene.Add(player1);

	auto const player2 = new QBert{ Tag::player2 };
	player2->AddObserver(obs2);
	scene.Add(player2);

	InputManager::GetInstance().SetUseKeyboard(true);
	InputManager::GetInstance().AddCommand(SDLK_a, new EarnPointsCommand{ KeyActionState::released, player1 });
	InputManager::GetInstance().AddCommand(SDLK_q, new EarnPointsCommand{ KeyActionState::pressed , player2 });
}