#include "PCH.h"
#include "MainGame.h"

#include "CharacterControllerSystem.h"
#include "CharacterMovementSystem.h"
#include "JumperSystem.h"
#include "PyramidComponent.h"

#include "CharacterControllerComponent.h"
#include "MovementComponent.h"
#include "JumpComponent.h"
#include "QubeComponent.h"


#include "Coily.h"
#include "CoopScene.h"
#include "ColoredDisk.h"
#include "MainMenuScene.h"

#include "QBert.h"

#include "VersusScene.h"
#include "SlickSam.h"
#include "SoloScene.h"
#include "WrongWay.h"
#include "WrongWayJumper.h"
#include "EnemyCharacterController.h"
#include "CoilyCharacterController.h"
#include "CharacterLives.h"
#include "CharacterPoint.h"
#include "QuitGameCommand.h"
#include "PauseGameCommand.h"
#include "ReloadSceneCommand.h"
#include "SwitchTextColor.h"
#include "SwitchScene.h"
#include "QBert.h"

#include "TextureRendererComponent.h"
#include "PrefabsManager.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"
#include "InputManager.h"
#include "JsonReaderWriter.h"
#include "ShapeRenderer.h"
#include "Shapes.h"
#include "BoxCollider.h"
#include "ButtonComponent.h"
#include "ShapeComponent.h"

#include "AudioComponent.h"

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
	SceneManager::GetInstance().LoadScene("MainMenuScene");
}

void MainGame::CreatePrefabs() const
{
	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 15);

	auto& pPrefabManager = PrefabsManager::GetInstance();

	//*auto snakeFallId = */ss->AddEffect("Data/Sounds/snake-fall.mp3");

	//lives
	auto const livesPrefab = pPrefabManager.CreatePrefab();
	auto livesObj = livesPrefab->GetRoot();
	livesObj->GetTransform()->Translate(20.f, 40.f);


	auto textRenderer = livesObj->AddComponent<TextRendererComponent>();
	textRenderer->SetText("P1 Lives: ");
	textRenderer->SetFont(font);
	textRenderer->SetTextColor(255, 0, 0);

	livesObj->AddComponent<TextureRendererComponent>()->m_Layer = 10;

	pPrefabManager.SavePrefab(livesPrefab, "LivesUI");

	//points
	auto const pointsrefab = pPrefabManager.CreatePrefab();
	auto const pointsObj = pointsrefab->GetRoot();
	pointsObj->GetTransform()->Translate(20.f, 60.f);

	textRenderer = pointsObj->AddComponent<TextRendererComponent>();
	textRenderer->SetText("P1 Points: 0 ");
	textRenderer->SetFont(font);

	pointsObj->AddComponent<TextureRendererComponent>()->m_Layer = 10;
	pPrefabManager.SavePrefab(pointsrefab, "PointsUI");

	//QBert
	auto qbertPrefab = pPrefabManager.CreatePrefab();
	auto qbert = qbertPrefab->GetRoot();

	auto rendererComp = qbert->AddComponent<TextureRendererComponent>();
	rendererComp->m_Layer = 8;
	rendererComp->m_pTexture = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert1_DownLeft_Qube.png");

	auto jumpSoundPtr = qbert->AddComponent<AudioComponent>();
	jumpSoundPtr->SetAudioClip("Sounds/jump.mp3");

	auto swearSoundPtr = qbert->AddComponent<AudioComponent>();
	swearSoundPtr->SetAudioClip("Sounds/swear.mp3");

	auto fallSoundPtr = qbert->AddComponent<AudioComponent>();
	fallSoundPtr->SetAudioClip("Sounds/fall.mp3");

	auto qBertComp = qbert->AddComponent<QBert>();
	qBertComp->SetAudioComponents(jumpSoundPtr, fallSoundPtr, swearSoundPtr);

	qbert->AddComponent<CharacterControllerComponent>();
	auto characterMovement = qbert->AddComponent<MovementComponent>();
	characterMovement->SetTextureIdleNames("Textures/QBert/QBert1_DownRight_Qube.png", "Textures/QBert/QBert1_DownLeft_Qube.png",
		"Textures/QBert/QBert1_UpRight_Qube.png", "Textures/QBert/QBert1_UpLeft_Qube.png");
	characterMovement->SetTextureJumpNames("Textures/QBert/QBert1_DownRight_Jump.png", "Textures/QBert/QBert1_DownLeft_Jump.png",
		"Textures/QBert/QBert1_UpRight_Jump.png", "Textures/QBert/QBert1_UpLeft_Jump.png");

	qbert->AddComponent<CharacterLives>();
	qbert->AddComponent<CharacterPoint>();
	qbert->AddComponent<JumpComponent>();
	//qbert->AddComponent(new BoxCollider{ 24,24 });
	auto hurtTextObj = qbertPrefab->CreateGameObject();

	auto hurtRenderer = hurtTextObj->AddComponent<TextureRendererComponent>();
	hurtRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture("Textures/QBert/HurtText.png");
	hurtRenderer->m_Layer = 8;
	hurtRenderer->SetActive(false);

	qbert->AddChild(hurtTextObj);
	hurtTextObj->GetTransform()->Translate(10, -20);
	qbert->GetTransform()->Scale(1.5f);

	qbertPrefab->AddSystem<CharacterControllerSystem>();
	qbertPrefab->AddSystem<CharacterMovementSystem>();
	qbertPrefab->AddSystem<JumperSystem>();

	pPrefabManager.SavePrefab(qbertPrefab, "QBert");

	//JsonReaderWriter* json = new JsonReaderWriter{ "./Data/Levels.json" };

	//Qube prefab
	auto qubePf = pPrefabManager.CreatePrefab();
	auto qubeObject = qubePf->GetRoot();
	qubeObject->GetTransform()->Scale(1.75f);
	qubeObject->AddComponent<TextureRendererComponent>()->m_Layer = 2;

	qubeObject->AddComponent<QubeComponent>();
	pPrefabManager.SavePrefab(qubePf, "Qube");

	//Pyramid
	//int levelWidth = json->ReadInt("width");
	auto pyramidPf = pPrefabManager.CreatePrefab();
	auto pyramidObject = pyramidPf->GetRoot();
	pyramidObject->GetTransform()->Translate(250.f, 400.f);
	//pyramid->AddComponent(new Pyramid{ (unsigned int)levelWidth});
	pyramidObject->AddComponent<PyramidSystem>();
	pPrefabManager.SavePrefab(pyramidPf, "Pyramid");

	////Ugg + WrongWay
	//auto wrongWayPrefab = pPrefabManager.CreatePrefab("WrongWay");
	//auto wrongWayObject = wrongWayPrefab->CreateGameObject();
	////wrongWayPrefab->AddComponent(new WrongWay{ true });
	//ECS_RendererComponent wrongWayRenderer;
	//wrongWayRenderer.m_Layer = Layer::middleground;

	//wrongWayObject->AddComponent<ECS_RendererComponent>(wrongWayRenderer);
	////wrongWayPrefab->AddComponent(new WrongWayJumper{});
	////wrongWayPrefab->AddComponent(new EnemyCharacterController{});
	////wrongWayPrefab->AddComponent(new BoxCollider{ 32,32 });
	//wrongWayObject->GetTransform()->Scale(2.f);

	////Coily prefab
	//auto coilyPrefab = pPrefabManager.CreatePrefab("Coily");
	//auto coilyObject = coilyPrefab->CreateGameObject();

	//ECS_RendererComponent coilyRenderer;
	//coilyRenderer.m_Layer = Layer::middleground;
	//coilyRenderer.m_pTexture = ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Egg_Small.png");

	//coilyObject->AddComponent<ECS_RendererComponent>(coilyRenderer);
	////coilyPrefab->AddComponent(new Coily{snakeFallId});
	////coilyPrefab->AddComponent(new CoilyCharacterController{});
	////coilyPrefab->AddComponent(new Jumper{});
	//coilyObject->GetTransform()->Scale(1.5f);


	////SlickSam
	//auto slickSamPf = pPrefabManager.CreatePrefab("SlickSam");
	//auto slickSamObject = slickSamPf->CreateGameObject();

	//ECS_RendererComponent slickSamRenderer;
	//slickSamRenderer.m_Layer = Layer::middleground;

	//slickSamObject->AddComponent<ECS_RendererComponent>(slickSamRenderer);
	////slickSamObject->AddComponent(new SlickSam{});
	////slickSamObject->AddComponent(new EnemyCharacterController{});
	////slickSamObject->AddComponent(new Jumper{});
	//slickSamObject->GetTransform()->Scale(1.5f);

	////ColoredDisk
	//auto diskPf = pPrefabManager.CreatePrefab("Disk");
	//auto diskObject = diskPf->CreateGameObject();

	////diskObject->AddComponent(new ColoredDisk{ });
	//ECS_RendererComponent diskRenderer;
	//diskRenderer.m_Layer = Layer::middleground;
	//diskRenderer.m_pTexture = ResourceManager::GetInstance().GetTexture("Textures/Disk.png");

	//diskObject->AddComponent<ECS_RendererComponent>(diskRenderer);
	//diskObject->GetTransform()->Scale(2);

	//Pause Menu
	auto const biggerFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 42);
	auto const lessBigFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 30);
	auto menuPrefab = pPrefabManager.CreatePrefab();
	auto menuObj = menuPrefab->GetRoot();

	auto shapeRenderer = menuObj->AddComponent<ShapeComponent>();
	shapeRenderer->SetShape(new geo::Rectangle{glm::vec2{0,0},Renderer::GetInstance().GetWindowWidth(), Renderer::GetInstance().GetWindowHeight(), Color{0,0,0, 127}, true} );
	shapeRenderer->m_Layer = 10;

	auto textObject = menuPrefab->CreateGameObject();

	auto textComp = textObject->AddComponent<TextRendererComponent>();
	textComp->SetText("Pause");
	textComp->SetFont(biggerFont);

	textObject->AddComponent<TextureRendererComponent>()->m_Layer = 11;
	menuObj->AddChild(textObject);
	textObject->GetTransform()->Translate(glm::vec2{ 400, 100 });

	auto btnObj = menuPrefab->CreateGameObject();
	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Resume");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<TextureRendererComponent>()->m_Layer = 11;

	auto resumeBtn = btnObj->AddComponent<ButtonComponent>();
	resumeBtn->m_Dimensions = { 110, 30 };
	resumeBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	resumeBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 200);
	btnObj->SetTag("ResumeBtn");

	//Back to main btn
	btnObj = menuPrefab->CreateGameObject();
	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Back to Main Menu");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<TextureRendererComponent>()->m_Layer = 11;

	auto backBtn = btnObj->AddComponent<ButtonComponent>();
	backBtn->m_Dimensions = { 110, 30 };
	backBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	backBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	backBtn->SetOnClickFunction(new SwitchScene{ "MainMenuScene" });

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 300);
	btnObj->SetTag("BackToMainBtn");

	//Quit Btn
	btnObj = menuPrefab->CreateGameObject();
	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Quit");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<TextureRendererComponent>()->m_Layer = 11;

	auto quitBtn = btnObj->AddComponent<ButtonComponent>();
	quitBtn->m_Dimensions = { 65, 30 };
	quitBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	quitBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	quitBtn->SetOnClickFunction(new QuitGameCommand{ });

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 400);
	btnObj->SetTag("QuitBtn");

	pPrefabManager.SavePrefab(menuPrefab, "PauseMenu");


	//Game over menu (opaque)
	auto quitMenuPrefab = pPrefabManager.CreatePrefab();
	menuObj = quitMenuPrefab->GetRoot();

	shapeRenderer = menuObj->AddComponent<ShapeComponent>();
	shapeRenderer->SetShape(new geo::Rectangle{ glm::vec2{0,0},Renderer::GetInstance().GetWindowWidth(), Renderer::GetInstance().GetWindowHeight(), Color{0,0,0, 255}, true });
	shapeRenderer->m_Layer = 10;

	textObject = quitMenuPrefab->CreateGameObject();

	textComp = textObject->AddComponent<TextRendererComponent>();
	textComp->SetText("Game Over");
	textComp->SetFont(biggerFont);

	textObject->AddComponent<TextureRendererComponent>()->m_Layer = 11;
	textObject->GetTransform()->Translate(glm::vec2{ 400, 100 });

	menuObj->AddChild(textObject);

	btnObj = quitMenuPrefab->CreateGameObject();

	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Replay");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<TextureRendererComponent>()->m_Layer = 11;
	auto replayBtn = btnObj->AddComponent<ButtonComponent>();
	replayBtn->m_Dimensions = { 100, 30 };
	replayBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	replayBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	replayBtn->SetOnClickFunction(new ReloadSceneCommand{});

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 200);
	btnObj->SetTag("ReplayBtn");

	//Back to main btn
	btnObj = quitMenuPrefab->CreateGameObject();

	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Back to Main Menu");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<TextureRendererComponent>()->m_Layer = 11;

	backBtn = btnObj->AddComponent<ButtonComponent>();
	backBtn->m_Dimensions = { 260, 30 };
	backBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	backBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	backBtn->SetOnClickFunction(new SwitchScene{ "MainMenuScene" });

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 300);
	btnObj->SetTag("BackToMainBtn");
	
	//Quit Btn
	btnObj = quitMenuPrefab->CreateGameObject();

	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Quit");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<TextureRendererComponent>()->m_Layer = 11;

	quitBtn = btnObj->AddComponent<ButtonComponent>();
	quitBtn->m_Dimensions = { 65, 30 };
	quitBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	quitBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	quitBtn->SetOnClickFunction(new QuitGameCommand{ });

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 400);
	btnObj->SetTag("QuitBtn");

	pPrefabManager.SavePrefab(quitMenuPrefab, "GameOverMenu");

	//delete json;
}