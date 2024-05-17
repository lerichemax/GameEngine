#include "PCH.h"
#include "MainGame.h"

#include "Coily.h"
#include "CoopScene.h"
#include "Jumper.h"
#include "ColoredDisk.h"
#include "MainMenuScene.h"
#include "TestScene.h"
#include "QBert.h"
#include "Qube.h"
#include "VersusScene.h"
#include "Pyramid.h"
#include "SlickSam.h"
#include "SoloScene.h"
#include "WrongWay.h"
#include "WrongWayJumper.h"
#include "EnemyCharacterController.h"
#include "CoilyCharacterController.h"
#include "CharacterLives.h"
#include "CharacterPoint.h"

#include "RendererComponent.h"
#include "PrefabsManager.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"
#include "InputManager.h"
#include "JsonReaderWriter.h"
#include "ShapeRenderer.h"
#include "Shapes.h"
#include "BoxCollider.h"
#include "ButtonComponent.h"
#include "SoundServiceLocator.h"

MainGame::MainGame()
	:NapoleonEngine()
{
	Initialize(900, 600, "QBert");
}

void MainGame::InitGame() const
{	
	//SceneManager::GetInstance().AddScene(new TestScene{});
	SceneManager::GetInstance().AddScene(new MainMenuScene{});
	SceneManager::GetInstance().AddScene(new SoloScene{});
	//SceneManager::GetInstance().AddScene(new CoopScene{});
	//SceneManager::GetInstance().AddScene(new VersusScene{});
	SceneManager::GetInstance().SetSceneActive("MainMenuScene");
}

void MainGame::CreatePrefabs() const
{
	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 15);

	auto& pPrefabManager = PrefabsManager::GetInstance();

	auto ss = new SoundSystem{false};
	/*auto jumpSoundid = */ss->AddEffect("Data/Sounds/jump.mp3");
	/*auto swearSoundId = */ss->AddEffect("Data/Sounds/swear.mp3");
	/*auto snakeFallId = */ss->AddEffect("Data/Sounds/snake-fall.mp3");
	/*auto fallsoundId = */ss->AddEffect("Data/Sounds/fall.mp3");

	SoundServiceLocator::Register(ss);

	ECS_RendererComponent rendererComp;
	rendererComp.m_Layer = Layer::uiGame;
	
	//lives
	auto const livesPrefab = pPrefabManager.CreatePrefab("LivesUI");
	auto livesObj = livesPrefab->CreateGameObject();
	livesObj->GetTransform()->Translate(20.f, 40.f);

	ECS_TextRendererComponent textRenderer{ "P1 Lives: ", font };
	textRenderer.SetTextColor(255, 0, 0);

	livesObj->AddComponent<ECS_TextRendererComponent>(textRenderer);
	livesObj->AddComponent<ECS_RendererComponent>(rendererComp);

	//points
	auto const pointsrefab = pPrefabManager.CreatePrefab("PointsUI");
	auto const pointsObj = pointsrefab->CreateGameObject();
	pointsObj->GetTransform()->Translate(20.f, 60.f);
	
	textRenderer.m_Text = "P1 Points: 0 ";
	pointsObj->AddComponent<ECS_TextRendererComponent>(textRenderer);
	pointsObj->AddComponent<ECS_RendererComponent>(rendererComp);


	//QBert
	auto qbertPrefab = pPrefabManager.CreatePrefab("QBert");
	auto qbert = qbertPrefab->CreateGameObject();
	rendererComp.m_Layer = Layer::foreground;
	qbert->AddComponent<ECS_RendererComponent>(rendererComp);
	//qbert->AddComponent(new QBert(jumpSoundid, fallsoundId, swearSoundId));
	//qbert->AddComponent(new CharacterLives{3});
	//qbert->AddComponent(new CharacterPoint{});
	//qbert->AddComponent(new Jumper{});
	//qbert->AddComponent(new BoxCollider{ 24,24 });
	//auto hurtTextObj = new GameObject;
	//hurtTextObj->AddComponent(new RendererComponent("Textures/QBert/HurtText.png", Layer::foreground));
	//qbert->AddChild(hurtTextObj);
	//hurtTextObj->GetECSTransform()->Translate(10, -20);
	//hurtTextObj->GetComponent<RendererComponent>()->SetEnable(false);
	qbert->GetTransform()->Scale(1.5f);

	JsonReaderWriter* json = new JsonReaderWriter{ "./Data/Levels.json" };
	
	//Qube prefab
	ECS_RendererComponent qubeRenderer;
	qubeRenderer.m_Layer = Layer::uiGame;

	auto qubePf = pPrefabManager.CreatePrefab("Qube");
	auto qubeObject = qubePf->CreateGameObject();
	auto text = ResourceManager::GetInstance().GetTexture(json->ReadString("initial texture"));
	auto interText = ResourceManager::GetInstance().GetTexture(json->ReadString("intermediate texture"));
	auto flippedText = ResourceManager::GetInstance().GetTexture(json->ReadString("flipped texture"));
	qubeObject->GetTransform()->Scale(1.75f);
	qubeRenderer.m_pTexture = text;
	qubeObject->AddComponent<ECS_RendererComponent>(qubeRenderer);
	//qubePf->AddComponent(new Qube{ text, interText, flippedText });

	//Pyramid
	//int levelWidth = json->ReadInt("width");
	auto pyramidPf = pPrefabManager.CreatePrefab("Pyramid");
	auto pyramidObject = pyramidPf->CreateGameObject();
	pyramidObject->GetTransform()->Translate(250.f, 400.f);
	//pyramid->AddComponent(new Pyramid{ (unsigned int)levelWidth});

	//Ugg + WrongWay
	auto wrongWayPrefab = pPrefabManager.CreatePrefab("WrongWay");
	auto wrongWayObject = wrongWayPrefab->CreateGameObject();
	//wrongWayPrefab->AddComponent(new WrongWay{ true });
	ECS_RendererComponent wrongWayRenderer;
	wrongWayRenderer.m_Layer = Layer::middleground;

	wrongWayObject->AddComponent<ECS_RendererComponent>(wrongWayRenderer);
	//wrongWayPrefab->AddComponent(new WrongWayJumper{});
	//wrongWayPrefab->AddComponent(new EnemyCharacterController{});
	//wrongWayPrefab->AddComponent(new BoxCollider{ 32,32 });
	wrongWayObject->GetTransform()->Scale(2.f);

	//Coily prefab
	auto coilyPrefab = pPrefabManager.CreatePrefab("Coily");
	auto coilyObject = coilyPrefab->CreateGameObject();

	ECS_RendererComponent coilyRenderer;
	coilyRenderer.m_Layer = Layer::middleground;
	coilyRenderer.m_pTexture = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Egg_Small.png");

	coilyObject->AddComponent<ECS_RendererComponent>(coilyRenderer);
	//coilyPrefab->AddComponent(new Coily{snakeFallId});
	//coilyPrefab->AddComponent(new CoilyCharacterController{});
	//coilyPrefab->AddComponent(new Jumper{});
	coilyObject->GetTransform()->Scale(1.5f);


	//SlickSam
	auto slickSamPf = pPrefabManager.CreatePrefab("SlickSam");
	auto slickSamObject = slickSamPf->CreateGameObject();

	ECS_RendererComponent slickSamRenderer;
	slickSamRenderer.m_Layer = Layer::middleground;

	slickSamObject->AddComponent<ECS_RendererComponent>(slickSamRenderer);
	//slickSamObject->AddComponent(new SlickSam{});
	//slickSamObject->AddComponent(new EnemyCharacterController{});
	//slickSamObject->AddComponent(new Jumper{});
	slickSamObject->GetTransform()->Scale(1.5f);

	//ColoredDisk
	auto diskPf = pPrefabManager.CreatePrefab("Disk");
	auto diskObject = diskPf->CreateGameObject();

	//diskObject->AddComponent(new ColoredDisk{ });
	ECS_RendererComponent diskRenderer;
	diskRenderer.m_Layer = Layer::middleground;
	diskRenderer.m_pTexture = ResourceManager::GetInstance().GetTexture("Textures/Disk.png");

	diskObject->AddComponent<ECS_RendererComponent>(diskRenderer);
	diskObject->GetTransform()->Scale(2);


	////Pause Menu
	//auto const biggerFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 42);
	//auto const lessBigFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 30);
	//auto menuObj = new GameObject{};
	//menuObj->AddComponent(new ShapeRenderer{ +
	//	new geo::Rectangle{glm::vec2{0,0},Renderer::GetInstance().GetWindowWidth(), Renderer::GetInstance().GetWindowHeight(), Color{0,0,0, 127}, true} });

	//auto textObject = new GameObject{}; 
	////auto textComp = new TextRendererComponent{ "Pause", biggerFont };
	////textComp->ChangeLayer(Layer::uiMenuFg);
	////textObject->AddComponent(textComp);
	//menuObj->AddChild(textObject);
	//textObject->GetECSTransform()->Translate(glm::vec2{ 400, 100 });

	//auto btnObj = new GameObject{};
	////textComp = new TextRendererComponent{ "Resume", lessBigFont };
	////textComp->ChangeLayer(Layer::uiMenuFg);
	//auto btn = new ButtonComponent{ 110, 30 };
	////btnObj->AddComponent(textComp);
	//btnObj->AddComponent(btn);
	//menuObj->AddChild(btnObj);
	//btnObj->GetECSTransform()->Translate(400, 200);
	//btnObj->SetTag("ResumeBtn",false);

	////Back to main btn
	//btnObj = new GameObject{};
	////textComp = new TextRendererComponent{ "Back to Main Menu", lessBigFont };
	////textComp->ChangeLayer(Layer::uiMenuFg);
	//btn = new ButtonComponent{ 260, 30 };
	////btnObj->AddComponent(textComp);
	//btnObj->AddComponent(btn);
	//menuObj->AddChild(btnObj);
	//btnObj->GetECSTransform()->Translate(400, 300);
	//btnObj->SetTag("BackToMainBtn", false);
	//
	////Quit Btn
	//btnObj = new GameObject{};
	////textComp = new TextRendererComponent{ "Quit", lessBigFont };
	////textComp->ChangeLayer(Layer::uiMenuFg);
	//btn = new ButtonComponent{ 65, 30 };
	////btnObj->AddComponent(textComp);
	//btnObj->AddComponent(btn);
	//menuObj->AddChild(btnObj);
	//btnObj->GetECSTransform()->Translate(400, 400);
	//btnObj->SetTag("QuitBtn", false);
	//
	//pPrefabManager.AddPrefab("PauseMenu", menuObj);
	//
	//
	////Game over menu (opaque)
	//menuObj = new GameObject{};
	//menuObj->AddComponent(new ShapeRenderer{ +
	//	new geo::Rectangle{glm::vec2{0,0},Renderer::GetInstance().GetWindowWidth(), Renderer::GetInstance().GetWindowHeight(), Color{0,0,0, 255}, true} });

	// textObject = new GameObject{};
	////textComp = new TextRendererComponent{ "Game Over", biggerFont };
	////textComp->ChangeLayer(Layer::uiMenuFg);
	////textObject->AddComponent(textComp);
	//menuObj->AddChild(textObject);
	//textObject->GetECSTransform()->Translate(glm::vec2{ 400, 100 });

	//btnObj = new GameObject{};
	////textComp = new TextRendererComponent{ "Replay", lessBigFont };
	////textComp->ChangeLayer(Layer::uiMenuFg);
	//btn = new ButtonComponent{ 100, 30 };
	////btnObj->AddComponent(textComp);
	//btnObj->AddComponent(btn);
	//menuObj->AddChild(btnObj);
	//btnObj->GetECSTransform()->Translate(400, 200);
	//btnObj->SetTag("ReplayBtn", false);

	////Back to main btn
	//btnObj = new GameObject{};
	////textComp = new TextRendererComponent{ "Back to Main Menu", lessBigFont };
	////textComp->ChangeLayer(Layer::uiMenuFg);
	//btn = new ButtonComponent{ 260, 30 };
	////btnObj->AddComponent(textComp);
	//btnObj->AddComponent(btn);
	//btnObj->SetTag("BackToMainBtn", false);
	//menuObj->AddChild(btnObj);
	//btnObj->GetECSTransform()->Translate(400, 300);
	//
	////Quit Btn
	//btnObj = new GameObject{};
	////textComp = new TextRendererComponent{ "Quit", lessBigFont };
	////textComp->ChangeLayer(Layer::uiMenuFg);
	//btn = new ButtonComponent{ 65, 30 };
	////btnObj->AddComponent(textComp);
	//btnObj->AddComponent(btn);
	//btnObj->SetTag("QuitBtn", false);
	//menuObj->AddChild(btnObj);
	//btnObj->GetECSTransform()->Translate(400, 400);
	//
	//pPrefabManager.AddPrefab("GameOverMenu", menuObj);

	delete json;
}

void MainGame::CreateMainMenuScene()
{
	//Scene* mainMenuScene = new Scene{ "MainMenuScene" };

	//auto* titleObject = new GameObject();
	//titleObject->AddComponent(new RendererComponent{ "Textures/UI/Title.png", Layer::uiGame });
	//Create(titleObject);
	//titleObject->GetTransform()->Translate(300, 50);
	//titleObject->GetTransform()->Scale(0.8f, 0.8f);
}