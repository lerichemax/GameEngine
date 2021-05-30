#include "PCH.h"
#include "MainGame.h"

#include "BoxCollider.h"
#include "Coily.h"
#include "CoopScene.h"
#include "Jumper.h"
#include "ColoredDisk.h"
#include "MainMenuScene.h"
#include "QBert.h"
#include "Qube.h"
#include "VersusScene.h"

#include "RendererComponent.h"
#include "PrefabsManager.h"
#include "ResourceManager.h"
#include "SlickSam.h"
#include "SoloScene.h"
#include "TextRendererComponent.h"
#include "WrongWay.h"
#include "WrongWayJumper.h"
#include "EnemyCharacterController.h"
#include "CoilyCharacterController.h"
#include "CharacterLives.h"
#include "CharacterPoint.h"
#include "InputManager.h"
#include "PauseGameCommand.h"

using namespace empire;

void MainGame::LoadGame() const
{
	InputManager::GetInstance().AddInputAction(20, new InputAction{ SDLK_ESCAPE, empire::KeyActionState::pressed,
		new PauseGameCommand() });

	InputManager::GetInstance().AddInputAction(21, new InputAction{ ControllerButton::Start, empire::KeyActionState::pressed,
		new PauseGameCommand() });
	
	SceneManager::GetInstance().AddScene(new MainMenuScene{});
	SceneManager::GetInstance().AddScene(new SoloScene{});
	SceneManager::GetInstance().AddScene(new CoopScene{});
	SceneManager::GetInstance().AddScene(new VersusScene{});
	SceneManager::GetInstance().SetSceneActive("MainMenuScene");
}

void MainGame::CreatePrefabs() const
{
	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 15);

	auto& pPrefabManager = PrefabsManager::GetInstance();
	
	auto const livesObj = new GameObject{};
	livesObj->GetTransform()->Translate(20.f, 40.f);
	livesObj->AddComponent(new TextRendererComponent{ "P1 Lives: ", font });
	livesObj->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	pPrefabManager.AddPrefab("LivesUI", livesObj);


	auto const pointsObj = new GameObject{};
	pointsObj->GetTransform()->Translate(20.f, 60.f);
	pointsObj->AddComponent(new TextRendererComponent{ "P1 Points: 0 ", font });
	pointsObj->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	pPrefabManager.AddPrefab("PointsUI", pointsObj);

	//QBert
	auto qbert = new GameObject();
	qbert->AddComponent(new RendererComponent( Layer::foreground));
	qbert->AddComponent(new QBert());
	qbert->AddComponent(new CharacterLives{3});
	qbert->AddComponent(new CharacterPoint{});
	qbert->AddComponent(new Jumper{});
	qbert->AddComponent(new BoxCollider{ 24,24 });
	qbert->GetTransform()->Scale(1.5f);
	pPrefabManager.AddPrefab("QBert", qbert);

	//Qube prefab
	auto qubePf = new GameObject{};
	auto text = ResourceManager::GetInstance().GetTexture("Qube.png");
	auto interText = ResourceManager::GetInstance().GetTexture("Qube_Intermediate.png");
	auto flippedText = ResourceManager::GetInstance().GetTexture("Qube_Flipped.png");
	qubePf->GetTransform()->Scale(1.75f);
	qubePf->AddComponent(new RendererComponent(text));
	qubePf->AddComponent(new Qube{ text, interText, flippedText });
	pPrefabManager.AddPrefab("Qube", qubePf);

	//Pyramid
	auto pyramid = new GameObject{};
	pyramid->GetTransform()->Translate(250.f, 400.f);
	pyramid->AddComponent(new Pyramid{ 7});
	pPrefabManager.AddPrefab("Pyramid", pyramid);

	//Ugg + WrongWay
	auto wrongWayPrefab = new GameObject{};
	wrongWayPrefab->AddComponent(new WrongWay{ true });
	wrongWayPrefab->AddComponent(new empire::RendererComponent{ empire::Layer::middleground });
	wrongWayPrefab->AddComponent(new WrongWayJumper{});
	wrongWayPrefab->AddComponent(new EnemyCharacterController{});
	wrongWayPrefab->AddComponent(new BoxCollider{ 32,32 });
	wrongWayPrefab->GetTransform()->Scale(2.f, 2.f);
	pPrefabManager.AddPrefab("WrongWay", wrongWayPrefab);

	//Coily prefab
	auto coilyPrefab = new GameObject{};
	auto pText = empire::ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Egg_Small.png");
	coilyPrefab->AddComponent(new empire::RendererComponent(pText, empire::Layer::middleground));
	coilyPrefab->AddComponent(new Coily{});
	coilyPrefab->AddComponent(new CoilyCharacterController{});
	coilyPrefab->AddComponent(new Jumper{});
	coilyPrefab->GetTransform()->Scale(1.5f);
	pPrefabManager.AddPrefab("Coily", coilyPrefab);

	//SlickSam
	auto slickSamPf = new GameObject();
	slickSamPf->AddComponent(new empire::RendererComponent(empire::Layer::middleground));
	slickSamPf->AddComponent(new SlickSam{});
	slickSamPf->AddComponent(new EnemyCharacterController{});
	slickSamPf->AddComponent(new Jumper{});
	slickSamPf->GetTransform()->Scale(1.5f);
	pPrefabManager.AddPrefab("SlickSam", slickSamPf);

	//ColoredDisk
	auto diskPf = new GameObject{};
	auto const diskText = empire::ResourceManager::GetInstance().GetTexture("Disk.png");
	diskPf->AddComponent(new ColoredDisk{ });
	diskPf->AddComponent(new empire::RendererComponent{ diskText, empire::Layer::middleground });
	diskPf->GetTransform()->Scale(2);
	pPrefabManager.AddPrefab("Disk", diskPf);
}