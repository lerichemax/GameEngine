#include "PCH.h"
#include "DemoScene.h"
#include "RendererComponent.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "TextRendererComponent.h"
#include "UIObject.h"
#include "PlayerObserver.h"
#include "QBert.h"
#include "Subject.h"
#include "EarnPointsCommand.h"
#include "PlayerDieCommand.h"
#include "SoundSystem.h"
#include "SoundCommands.h"
#include "ServiceLocator.h"
#include "LoggingSoundSystem.h"
#include "FPSCounter.h"

using namespace empire;

DemoScene::DemoScene() : Scene("DemoScene")
{
}

void DemoScene::Initialize()
{
	
	//auto* go = new GameObject{  };
	//go->AddComponent(new RendererComponent("background.jpg"));
	//Add(go);

	//go = new GameObject{ };
	//go->AddComponent(new RendererComponent("logo.png"));
	//go->GetTransform()->Translate(216.f, 180.f);
	//Add(go);

	//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = new GameObject{ };
	//to->AddComponent(new TextRendererComponent("Programming 4 Assignment", font));
	//to->GetTransform()->Translate(80.f, 20.f);
	//Add(to);

	//auto const pointsP1 = new UIObject{};
	//pointsP1->GetComponent<TransformComponent>()->Translate(20.f, 50.f, 0.f);
	//pointsP1->GetComponent<TextRendererComponent>()->SetText("P1 Points: 0 ");
	//pointsP1->GetComponent<TextRendererComponent>()->SetTextColor(0, 0, 255);
	//Add(pointsP1);

	//auto const livesP1 = new UIObject{};
	//livesP1->GetComponent<TransformComponent>()->Translate(20.f, 70.f, 0.f);
	//livesP1->GetComponent<TextRendererComponent>()->SetTextColor(0, 0, 255);
	//Add(livesP1);

	//auto const pointsP2 = new UIObject{};
	//pointsP2->GetComponent<TransformComponent>()->Translate(20.f, 100.f, 0.f);
	//pointsP2->GetComponent<TextRendererComponent>()->SetText("P2 Points: 0 ");
	//pointsP2->GetComponent<TextRendererComponent>()->SetTextColor(0, 255, 0);
	//Add(pointsP2);

	//auto const livesP2 = new UIObject{};
	//livesP2->GetComponent<TransformComponent>()->Translate(20.f, 120.f, 0.f);
	//livesP2->GetComponent<TextRendererComponent>()->SetTextColor(0, 255, 0);
	//Add(livesP2);

	///*auto obs1 = new PlayerObserver{ pointsP1, livesP1 };
	//auto obs2 = new PlayerObserver{ pointsP2, livesP2 };*/

	////auto const player1 = new GameObject{ };
	////player1->AddComponent(new QBert{});
	////player1->GetComponent<QBert>()->GetSubject()->AddObserver(obs1);
	////livesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + std::to_string(player1->GetComponent<QBert>()->GetLives()));
	////Add(player1);

	////auto const player2 = new GameObject{  };
	////player2->AddComponent(new QBert{});
	////player2->GetComponent<QBert>()->GetSubject()->AddObserver(obs2);
	////livesP2->GetComponent<TextRendererComponent>()->SetText("P2 Lives: " + std::to_string(player2->GetComponent<QBert>()->GetLives()));
	////Add(player2);

	//auto fpsCounter = new FPSCounter{};
	//Add(fpsCounter);

	//InputManager::GetInstance().SetUseKeyboard(true);
	//InputManager::GetInstance().AddCommand(SDLK_a, new EarnPointsCommand{ KeyActionState::pressed, player1 });
	//InputManager::GetInstance().AddCommand(SDLK_q, new EarnPointsCommand{ KeyActionState::pressed , player2 });
	//InputManager::GetInstance().AddCommand(SDLK_w, new PlayerDieCommand{ KeyActionState::pressed, player1 });
	//InputManager::GetInstance().AddCommand(SDLK_s, new PlayerDieCommand{ KeyActionState::pressed , player2 });

	////auto ss = new SoundSystem{};
	////auto id1 = ss->AddEffect("../Data/Sounds/jump.mp3");
	//auto lss = new SoundSystem{};
	//auto id2 = lss->AddEffect("../Data/Sounds/fall.mp3");

	////ServiceLocator::Register(ss);
	//ServiceLocator::Register(new LoggingSoundSystem{ lss, false });

	////InputManager::GetInstance().AddCommand(SDLK_o, new PlaySoundCommand{ id1, 50, KeyActionState::pressed });
	//InputManager::GetInstance().AddCommand(SDLK_p, new PlaySoundCommand{ id2, 50, KeyActionState::pressed });
}