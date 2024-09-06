#include "PCH.h"
#include "MainGame.h"

#include "CharacterControllerSystem.h"
#include "CharacterMovementSystem.h"
#include "JumperSystem.h"
#include "QubeSystem.h"
#include "PyramidSystem.h"
#include "QBertSystem.h"
#include "DiskSystem.h"
#include "AiControllerSystem.h"

#include "CharacterControllerComponent.h"
#include "CharacterComponent.h"
#include "MovementComponent.h"
#include "JumpComponent.h"
#include "QubeComponent.h"
#include "PyramidComponent.h"
#include "QbertComponent.h"
#include "DiskComponent.h"
#include "AiControllerComponent.h"

#include "Coily.h"
#include "CoopScene.h"
#include "MainMenuScene.h"

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


#include "PrefabsManager.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"
#include "InputManager.h"
#include "JsonReaderWriter.h"
#include "Shapes.h"
#include "BoxCollider.h"
#include "ButtonComponent.h"

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

	livesObj->AddComponent<RendererComponent>()->Layer = 10;

	pPrefabManager.SavePrefab(livesPrefab, "LivesUI");

	//points
	auto const pointsrefab = pPrefabManager.CreatePrefab();
	auto const pointsObj = pointsrefab->GetRoot();
	pointsObj->GetTransform()->Translate(20.f, 60.f);

	textRenderer = pointsObj->AddComponent<TextRendererComponent>();
	textRenderer->SetText("P1 Points: 0 ");
	textRenderer->SetFont(font);

	pointsObj->AddComponent<RendererComponent>()->Layer = 10;
	pPrefabManager.SavePrefab(pointsrefab, "PointsUI");

	//QBert
	auto qbertPrefab = pPrefabManager.CreatePrefab();
	auto pQbertObj = qbertPrefab->GetRoot();

	auto rendererComp = pQbertObj->AddComponent<RendererComponent>();
	rendererComp->Layer = 8;
	rendererComp->pTexture = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert1_DownLeft_Qube.png");

	auto* const jumpSoundPtr = pQbertObj->AddComponent<AudioComponent>();
	jumpSoundPtr->SetAudioClip("Sounds/jump.mp3");

	auto* const swearSoundPtr = pQbertObj->AddComponent<AudioComponent>();
	swearSoundPtr->SetAudioClip("Sounds/swear.mp3");

	auto* const fallSoundPtr = pQbertObj->AddComponent<AudioComponent>();
	fallSoundPtr->SetAudioClip("Sounds/fall.mp3");

	auto* const pQbertComp = pQbertObj->AddComponent<QbertComponent>();
	pQbertComp->pFallSound = fallSoundPtr;
	pQbertComp->pSwearSound = swearSoundPtr;
	pQbertComp->pJumpSound = jumpSoundPtr;


	pQbertObj->AddComponent<CharacterControllerComponent>();
	auto* const pCharacterMovement = pQbertObj->AddComponent<MovementComponent>();
	pCharacterMovement->SetTextureIdleNames("Textures/QBert/QBert1_DownRight_Qube.png", "Textures/QBert/QBert1_DownLeft_Qube.png",
		"Textures/QBert/QBert1_UpRight_Qube.png", "Textures/QBert/QBert1_UpLeft_Qube.png");
	pCharacterMovement->SetTextureJumpNames("Textures/QBert/QBert1_DownRight_Jump.png", "Textures/QBert/QBert1_DownLeft_Jump.png",
		"Textures/QBert/QBert1_UpRight_Jump.png", "Textures/QBert/QBert1_UpLeft_Jump.png");

	pQbertObj->AddComponent<CharacterLives>();
	pQbertObj->AddComponent<CharacterPoint>();
	pQbertObj->AddComponent<JumpComponent>();
	//qbert->AddComponent(new BoxCollider{ 24,24 });
	auto hurtTextObj = qbertPrefab->CreateGameObject();

	auto hurtRenderer = hurtTextObj->AddComponent<RendererComponent>();
	hurtRenderer->pTexture = ResourceManager::GetInstance().GetTexture("Textures/QBert/HurtText.png");
	hurtRenderer->Layer = 8;
	hurtRenderer->SetActive(false);

	pQbertObj->AddChild(hurtTextObj);
	hurtTextObj->GetTransform()->Translate(10, -20);
	pQbertObj->GetTransform()->Scale(1.5f);
	pQbertObj->SetTag(QBERT_TAG);

	qbertPrefab->AddRequiredSystem<CharacterControllerSystem>();
	qbertPrefab->AddRequiredSystem<CharacterMovementSystem>();
	qbertPrefab->AddRequiredSystem<JumperSystem>();
	qbertPrefab->AddRequiredSystem<QBertSystem>();

	pPrefabManager.SavePrefab(qbertPrefab, "QBert");

	//JsonReaderWriter* json = new JsonReaderWriter{ "./Data/Levels.json" };

	//Qube prefab
	auto qubePf = pPrefabManager.CreatePrefab();
	auto qubeObject = qubePf->GetRoot();
	qubeObject->GetTransform()->Scale(1.75f);
	qubeObject->AddComponent<QubeComponent>();
	qubeObject->AddComponent<RendererComponent>()->Layer = 2;
	qubePf->AddRequiredSystem<QubeSystem>();
	pPrefabManager.SavePrefab(qubePf, "Qube");

	//Pyramid
	//int levelWidth = json->ReadInt("width");
	auto pyramidPf = pPrefabManager.CreatePrefab();
	auto pyramidObject = pyramidPf->GetRoot();
	pyramidObject->GetTransform()->Translate(250.f, 400.f);
	pyramidObject->AddComponent<PyramidComponent>();
	pyramidPf->AddRequiredSystem<PyramidSystem>();
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


	//SlickSam
	auto pSlickPf = pPrefabManager.CreatePrefab(); //slick
	auto pSlickObject = pSlickPf->GetRoot();

	auto* const pSlickRenderer = pSlickObject->AddComponent<RendererComponent>();
	pSlickRenderer->Layer = 7;
	pSlickRenderer->pTexture = ResourceManager::GetInstance().GetTexture("Textures/Enemies/SlickSam/Slick_Down_Right.png");
	//slickSamObject->AddComponent(new SlickSam{});
	//slickSamObject->AddComponent(new EnemyCharacterController{});
	auto* const pMover = pSlickObject->AddComponent<MovementComponent>();
	pMover->SetTextureIdleNames("Textures/Enemies/SlickSam/Slick_Down_Right.png", "Textures/Enemies/SlickSam/Slick_Down_Left.png","", "");
	pMover->SetTextureJumpNames("Textures/Enemies/SlickSam/Slick_Up_Right.png", "Textures/Enemies/SlickSam/Slick_Up_Left","", "");

	pSlickObject->AddComponent<JumpComponent>();
	pSlickObject->AddComponent<MovementComponent>();
	pSlickObject->AddComponent<AiControllerComponent>()->Type = EnemyType::SlickSam;
	pSlickObject->GetTransform()->Scale(1.5f);
	pSlickObject->SetTag(ENEMY_TAG);

	pSlickPf->AddRequiredSystem<AiControllerSystem>();

	pPrefabManager.SavePrefab(pSlickPf, "Slick");

	auto pSamPf = pPrefabManager.CreatePrefab(); //sam	
	auto pSamObject = pSamPf->GetRoot();

	auto* const pSamRenderer = pSamObject->AddComponent<RendererComponent>();
	pSamRenderer->Layer = 7;
	pSamRenderer->pTexture = ResourceManager::GetInstance().GetTexture("Textures/Enemies/SlickSam/Slick_Down_Right.png");
	pSamRenderer->pTexture = ResourceManager::GetInstance().GetTexture("Textures/Enemies/SlickSam/Sam_Down_Right.png");
	pMover->SetTextureIdleNames("Textures/Enemies/SlickSam/Sam_Down_Right.png", "Textures/Enemies/SlickSam/Sam_Down_Left.png", "", "");
	pMover->SetTextureJumpNames("Textures/Enemies/SlickSam/Sam_Up_Right.png", "Textures/Enemies/SlickSam/Sam_Up_Left", "", "");

	pSamObject->AddComponent<JumpComponent>();
	pSamObject->AddComponent<MovementComponent>();
	pSamObject->AddComponent<AiControllerComponent>()->Type = EnemyType::SlickSam;
	pSamObject->GetTransform()->Scale(1.5f);
	pSamObject->SetTag(ENEMY_TAG);

	pSamPf->AddRequiredSystem<AiControllerSystem>();

	pPrefabManager.SavePrefab(pSamPf, "Sam");

	//Disks
	auto pDiskPf = pPrefabManager.CreatePrefab();
	auto pDiskObject = pDiskPf->GetRoot();

	pDiskObject->AddComponent<DiskComponent>();
	auto* const pDiskRenderer = pDiskObject->AddComponent<RendererComponent>();
	pDiskRenderer->Layer = 7;
	pDiskRenderer->pTexture = ResourceManager::GetInstance().GetTexture("Textures/Disk.png");

	pDiskObject->GetTransform()->Scale(2);

	pDiskPf->AddRequiredSystem<DiskSystem>();
	pPrefabManager.SavePrefab(pDiskPf, "Disk");

	//Pause Menu
	auto const biggerFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 42);
	auto const lessBigFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 30);
	auto menuPrefab = pPrefabManager.CreatePrefab();
	auto menuObj = menuPrefab->GetRoot();

	auto* pShapeRenderer = menuObj->AddComponent<RendererComponent>();
	pShapeRenderer->SetShape(new geo::Rectangle{glm::vec2{0,0},Renderer::GetInstance().GetWindowWidth(), Renderer::GetInstance().GetWindowHeight(), Color{0,0,0, 127}, true} );
	pShapeRenderer->Layer = 10;

	auto textObject = menuPrefab->CreateGameObject();

	auto textComp = textObject->AddComponent<TextRendererComponent>();
	textComp->SetText("Pause");
	textComp->SetFont(biggerFont);

	textObject->AddComponent<RendererComponent>()->Layer = 11;
	menuObj->AddChild(textObject);
	textObject->GetTransform()->Translate(glm::vec2{ 400, 100 });

	auto btnObj = menuPrefab->CreateGameObject();
	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Resume");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<RendererComponent>()->Layer = 11;

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

	btnObj->AddComponent<RendererComponent>()->Layer = 11;

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

	btnObj->AddComponent<RendererComponent>()->Layer = 11;

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

	pShapeRenderer = menuObj->AddComponent<RendererComponent>();
	pShapeRenderer->SetShape(new geo::Rectangle{ glm::vec2{0,0},Renderer::GetInstance().GetWindowWidth(), Renderer::GetInstance().GetWindowHeight(), Color{0,0,0, 255}, true });
	pShapeRenderer->Layer = 10;

	textObject = quitMenuPrefab->CreateGameObject();

	textComp = textObject->AddComponent<TextRendererComponent>();
	textComp->SetText("Game Over");
	textComp->SetFont(biggerFont);

	textObject->AddComponent<RendererComponent>()->Layer = 11;
	textObject->GetTransform()->Translate(glm::vec2{ 400, 100 });

	menuObj->AddChild(textObject);

	btnObj = quitMenuPrefab->CreateGameObject();

	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Replay");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<RendererComponent>()->Layer = 11;
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

	btnObj->AddComponent<RendererComponent>()->Layer = 11;

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

	btnObj->AddComponent<RendererComponent>()->Layer = 11;

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