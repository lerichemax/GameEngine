#include "PCH.h"
#include "MainGame.h"

#include "AudioComponent.h"
#include "TextRendererComponent.h"
#include "ButtonComponent.h"
#include "CharacterControllerComponent.h"
#include "MovementComponent.h"
#include "JumpComponent.h"
#include "QubeComponent.h"
#include "PyramidComponent.h"
#include "QbertComponent.h"
#include "DiskComponent.h"
#include "AiControllerComponent.h"
#include "CoilyComponent.h"
#include "ScriptComponent.h"
#include "CharacterLives.h"
#include "CharacterPoint.h"
#include "ColliderComponent.h"
#include "EnemySpawnerComponent.h"
#include "UiManagerComponent.h"

#include "CoopScene.h"
#include "MainMenuScene.h"
#include "VersusScene.h"
#include "SoloScene.h"

#include "QuitGameCommand.h"
#include "PauseGameCommand.h"
#include "ReloadSceneCommand.h"
#include "SwitchTextColor.h"
#include "SwitchScene.h"

#include "PrefabsManager.h"
#include "ResourceManager.h"
#include "InputManager.h"

#include "Shapes.h"


MainGame::MainGame()
	:NapoleonEngine(900, 600, "QBert", false)
{
}

void MainGame::InitGame() const
{
	SceneManager::Get().AddScene(new MainMenuScene{});
	SceneManager::Get().AddScene(new SoloScene{});
	SceneManager::Get().AddScene(new CoopScene{});
	SceneManager::Get().AddScene(new VersusScene{});
	SceneManager::Get().LoadScene("MainMenuScene");
}

void MainGame::CreatePrefabs() const
{
	auto const font = ResourceManager::Get().GetFont("Fonts/Lingua.otf", 15);

	auto& pPrefabManager = PrefabsManager::Get();

	//lives
	auto const livesPrefab = pPrefabManager.CreatePrefab();
	auto livesObj = livesPrefab->CreateGameObject();
	livesObj->GetTransform()->SetLocation(20.f, 40.f);


	auto textRenderer = livesObj->AddComponent<TextRendererComponent>();
	textRenderer->SetText("P1 Lives: ");
	textRenderer->SetFont(font);
	textRenderer->SetTextColor(255, 0, 0);

	livesObj->AddComponent<RendererComponent>()->Layer = 10;

	pPrefabManager.SavePrefab(livesPrefab, "LivesUI");

	//points
	auto const pointsrefab = pPrefabManager.CreatePrefab();
	auto const pointsObj = pointsrefab->CreateGameObject();
	pointsObj->GetTransform()->SetLocation(20.f, 60.f);

	textRenderer = pointsObj->AddComponent<TextRendererComponent>();
	textRenderer->SetText("P1 Points: 0 ");
	textRenderer->SetFont(font);

	pointsObj->AddComponent<RendererComponent>()->Layer = 10;
	pPrefabManager.SavePrefab(pointsrefab, "PointsUI");

	//QBert
	auto qbertPrefab = pPrefabManager.CreatePrefab();
	auto pQbertObj = qbertPrefab->CreateGameObject();

	auto rendererComp = pQbertObj->AddComponent<RendererComponent>();
	rendererComp->Layer = 8;
	rendererComp->pTexture = ResourceManager::Get().GetTexture("Textures/QBert/QBert1_DownLeft_Qube.png");

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

	auto pCharacterController = pQbertObj->AddComponent<CharacterControllerComponent>();
	pCharacterController->Right = SDL_SCANCODE_D;
	pCharacterController->Left = SDL_SCANCODE_A;
	pCharacterController->Up = SDL_SCANCODE_W;
	pCharacterController->Down = SDL_SCANCODE_S;
	pCharacterController->PlayerNumber = 1;

	auto* const pCharacterMovement = pQbertObj->AddComponent<MovementComponent>();
	pCharacterMovement->SetTextureIdleNames("Textures/QBert/QBert1_DownRight_Qube.png", "Textures/QBert/QBert1_DownLeft_Qube.png",
		"Textures/QBert/QBert1_UpRight_Qube.png", "Textures/QBert/QBert1_UpLeft_Qube.png");
	pCharacterMovement->SetTextureJumpNames("Textures/QBert/QBert1_DownRight_Jump.png", "Textures/QBert/QBert1_DownLeft_Jump.png",
		"Textures/QBert/QBert1_UpRight_Jump.png", "Textures/QBert/QBert1_UpLeft_Jump.png");
	pCharacterMovement->Mode = MovementMode::Normal;

	pQbertObj->AddComponent<CharacterLives>()->Init(3);
	pQbertObj->AddComponent<CharacterPoint>();
	pQbertObj->AddComponent<JumpComponent>()->Direction = {0, -1};

	auto* pCollider = pQbertObj->AddComponent<ColliderComponent>();
	pCollider->SetShape(new geo::Rectangle{ pQbertObj->GetTransform()->GetLocation(), 24,24, {255, 0, 0} });
	pCollider->bIsTrigger = true;

	auto pHurtTextObj = qbertPrefab->CreateGameObject();

	auto pHurtRenderer = pHurtTextObj->AddComponent<RendererComponent>();
	pHurtRenderer->pTexture = ResourceManager::Get().GetTexture("Textures/QBert/HurtText.png");
	pHurtRenderer->Layer = 8;
	pHurtRenderer->SetActive(false);

	pQbertObj->AddChild(pHurtTextObj);
	pHurtTextObj->GetTransform()->SetLocation(-10, -34);
	pQbertObj->GetTransform()->Scale(1.5f);
	pQbertObj->SetTag(QBERT_TAG);

	pPrefabManager.SavePrefab(qbertPrefab, "QBert");


	//Qube prefab
	auto qubePf = pPrefabManager.CreatePrefab();
	auto qubeObject = qubePf->CreateGameObject();
	qubeObject->GetTransform()->Scale(1.75f);
	qubeObject->AddComponent<QubeComponent>();
	qubeObject->AddComponent<RendererComponent>()->Layer = 2;

	pPrefabManager.SavePrefab(qubePf, "Qube");

	//Pyramid
	auto pyramidPf = pPrefabManager.CreatePrefab();
	auto pyramidObject = pyramidPf->CreateGameObject();
	pyramidObject->GetTransform()->SetLocation(250.f, 400.f);
	pyramidObject->AddComponent<PyramidComponent>();
	pPrefabManager.SavePrefab(pyramidPf, "Pyramid");

	//Ugg + WrongWay
	auto pUggPrefab = pPrefabManager.CreatePrefab();
	auto pUggobject = pUggPrefab->CreateGameObject();
	auto* const pUggRenderer = pUggobject->AddComponent<RendererComponent>();
	pUggRenderer->Layer = 7;
	pUggRenderer->pTexture = ResourceManager::Get().GetTexture("Textures/Enemies/WrongWay/Ugg_Right.png");

	auto* pMover = pUggobject->AddComponent<MovementComponent>();
	pMover->SetTextureIdleNames("Textures/Enemies/WrongWay/Ugg_Right.png", "Textures/Enemies/WrongWay/Ugg_left.png", "", "");
	pMover->SetTextureJumpNames("Textures/Enemies/WrongWay/Ugg_Right.png", "Textures/Enemies/WrongWay/Ugg_left.png", "", "");
	pMover->Mode = MovementMode::EscheresqueLeft;

	pUggobject->AddComponent<JumpComponent>()->Direction = { -1, 0 };
	pUggobject->AddComponent<CharacterLives>()->Init(1);

	pCollider = pUggobject->AddComponent<ColliderComponent>();
	pCollider->SetShape(new geo::Rectangle{ pUggobject->GetTransform()->GetLocation(), 32,32, {255, 0, 0} });

	auto* pAiController = pUggobject->AddComponent<AiControllerComponent>();
	pAiController->Type = EnemyType::WrongWay;
	pAiController->PointsForKill = 0;

	pUggobject->SetTag(ENEMY_TAG);
	pUggobject->GetTransform()->Scale(1.5f);

	pPrefabManager.SavePrefab(pUggPrefab, "Ugg");

	auto pWrongWayPrefab = pPrefabManager.CreatePrefab();
	auto pWrongWayObject = pWrongWayPrefab->CreateGameObject();
	auto* const pWrongWayRenderer = pWrongWayObject->AddComponent<RendererComponent>();
	pWrongWayRenderer->Layer = 7;
	pWrongWayRenderer->pTexture = ResourceManager::Get().GetTexture("Textures/Enemies/WrongWay/WrongWay_Right.png");

	pMover = pWrongWayObject->AddComponent<MovementComponent>();
	pMover->SetTextureIdleNames("Textures/Enemies/WrongWay/WrongWay_Right.png", "Textures/Enemies/WrongWay/WrongWay_left.png", "", "");
	pMover->SetTextureJumpNames("Textures/Enemies/WrongWay/WrongWay_Right.png", "Textures/Enemies/WrongWay/WrongWay_left.png", "", "");
	pMover->Mode = MovementMode::EscheresqueRight;

	pWrongWayObject->AddComponent<JumpComponent>()->Direction = { 1, 0 };
	pWrongWayObject->AddComponent<CharacterLives>()->Init(1);

	pCollider = pWrongWayObject->AddComponent<ColliderComponent>();
	pCollider->SetShape(new geo::Rectangle{ pWrongWayObject->GetTransform()->GetLocation(), 32,32, {255, 0, 0} });

	pAiController = pWrongWayObject->AddComponent<AiControllerComponent>();
	pAiController->Type = EnemyType::WrongWay;
	pAiController->PointsForKill = 0;

	pWrongWayObject->SetTag(ENEMY_TAG);
	pWrongWayObject->GetTransform()->Scale(2.f);

	pPrefabManager.SavePrefab(pWrongWayPrefab, "WrongWay");

	//Coily prefab
	auto pCoilyPrefab = pPrefabManager.CreatePrefab();
	auto pCoilyObject = pCoilyPrefab->CreateGameObject();

	auto* const pCoilyRenderer = pCoilyObject->AddComponent<RendererComponent>();

	pCoilyRenderer->Layer = 7;
	pCoilyRenderer->pTexture = ResourceManager::Get().GetTexture("Textures/Enemies/Coily/Coily_Egg_Small.png");

	pMover = pCoilyObject->AddComponent<MovementComponent>();
	pMover->SetTextureIdleNames("Textures/Enemies/Coily/Coily_Egg_Small.png", "Textures/Enemies/Coily/Coily_Egg_Small.png", "", "");
	pMover->SetTextureJumpNames("Textures/Enemies/Coily/Coily_Egg_Big.png", "Textures/Enemies/Coily/Coily_Egg_Big.png", "", "");
	pMover->Mode = MovementMode::Normal;

	pCoilyObject->AddComponent<JumpComponent>()->Direction = { 0, -1 };;
	pCoilyObject->AddComponent<CharacterLives>()->Init(1);

	pAiController = pCoilyObject->AddComponent<AiControllerComponent>();
	pAiController->Type = EnemyType::Coily;
	pAiController->PointsForKill = 500;

	pCoilyObject->AddComponent<CoilyComponent>();
	pCoilyObject->AddComponent<AudioComponent>()->SetAudioClip("Sounds/snake-fall.mp3");
	pCoilyObject->SetTag(ENEMY_TAG);
	pCoilyObject->GetTransform()->Scale(1.5f);

	pPrefabManager.SavePrefab(pCoilyPrefab, "Coily");

	//SlickSam
	auto pSlickPf = pPrefabManager.CreatePrefab(); //slick
	auto pSlickObject = pSlickPf->CreateGameObject();

	auto* const pSlickRenderer = pSlickObject->AddComponent<RendererComponent>();
	pSlickRenderer->Layer = 7;
	pSlickRenderer->pTexture = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Slick_Down_Right.png");
	pMover = pSlickObject->AddComponent<MovementComponent>();
	pMover->SetTextureIdleNames("Textures/Enemies/SlickSam/Slick_Down_Right.png", "Textures/Enemies/SlickSam/Slick_Down_Left.png","", "");
	pMover->SetTextureJumpNames("Textures/Enemies/SlickSam/Slick_Up_Right.png", "Textures/Enemies/SlickSam/Slick_Up_Left","", "");
	pMover->Mode = MovementMode::Normal;

	pSlickObject->AddComponent<JumpComponent>()->Direction = { 0, -1 };
	pSlickObject->AddComponent<CharacterLives>()->Init(1);

	pAiController = pSlickObject->AddComponent<AiControllerComponent>();
	pAiController->Type = EnemyType::SlickSam;
	pAiController->PointsForKill = 300;

	pSlickObject->GetTransform()->Scale(1.5f);
	pSlickObject->SetTag(ENEMY_TAG);

	pPrefabManager.SavePrefab(pSlickPf, "Slick");

	auto pSamPf = pPrefabManager.CreatePrefab(); //sam	
	auto pSamObject = pSamPf->CreateGameObject();

	auto* const pSamRenderer = pSamObject->AddComponent<RendererComponent>();
	pSamRenderer->Layer = 7;
	pSamRenderer->pTexture = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Slick_Down_Right.png");
	pSamRenderer->pTexture = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Sam_Down_Right.png");

	pMover = pSamObject->AddComponent<MovementComponent>();
	pMover->SetTextureIdleNames("Textures/Enemies/SlickSam/Sam_Down_Right.png", "Textures/Enemies/SlickSam/Sam_Down_Left.png", "", "");
	pMover->SetTextureJumpNames("Textures/Enemies/SlickSam/Sam_Up_Right.png", "Textures/Enemies/SlickSam/Sam_Up_Left", "", "");
	pMover->Mode = MovementMode::Normal;

	pSamObject->AddComponent<JumpComponent>()->Direction = { 0, -1 };
	pSamObject->AddComponent<CharacterLives>()->Init(1);

	pAiController = pSamObject->AddComponent<AiControllerComponent>();
	pAiController->Type = EnemyType::SlickSam;
	pAiController->PointsForKill = 300;

	pSamObject->GetTransform()->Scale(1.5f);
	pSamObject->SetTag(ENEMY_TAG);

	pPrefabManager.SavePrefab(pSamPf, "Sam");

	//Disks
	auto pDiskPf = pPrefabManager.CreatePrefab();
	auto pDiskObject = pDiskPf->CreateGameObject();

	pDiskObject->AddComponent<DiskComponent>();
	auto* const pDiskRenderer = pDiskObject->AddComponent<RendererComponent>();
	pDiskRenderer->Layer = 7;
	pDiskRenderer->pTexture = ResourceManager::Get().GetTexture("Textures/Disk.png");

	pDiskObject->GetTransform()->Scale(2);

	pPrefabManager.SavePrefab(pDiskPf, "Disk");

	//Pause Menu
	auto const biggerFont = ResourceManager::Get().GetFont("Fonts/Lingua.otf", 42);
	auto const lessBigFont = ResourceManager::Get().GetFont("Fonts/Lingua.otf", 30);
	auto menuPrefab = pPrefabManager.CreatePrefab();
	auto menuObj = menuPrefab->CreateGameObject();

	auto* pShapeRenderer = menuObj->AddComponent<RendererComponent>();
	pShapeRenderer->SetShape(new geo::Rectangle{glm::vec2{0,0},NapoleonEngine::GetEngine()->GetWindowWidth(), NapoleonEngine::GetEngine()->GetWindowHeight(), Color{0,0,0, 127}, true});
	pShapeRenderer->Layer = 10;

	auto textObject = menuPrefab->CreateGameObject();

	auto textComp = textObject->AddComponent<TextRendererComponent>();
	textComp->SetText("Pause");
	textComp->SetFont(biggerFont);

	textObject->AddComponent<RendererComponent>()->Layer = 11;
	menuObj->AddChild(textObject);
	textObject->GetTransform()->SetLocation(glm::vec2{ 400, 100 });

	auto btnObj = menuPrefab->CreateGameObject();
	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Resume");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<RendererComponent>()->Layer = 11;

	auto resumeBtn = btnObj->AddComponent<ButtonComponent>();
	resumeBtn->Dimensions = { 110, 30 };
	resumeBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	resumeBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->SetLocation(400, 200);
	btnObj->SetTag("ResumeBtn");

	//Back to main btn
	btnObj = menuPrefab->CreateGameObject();
	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Back to Main Menu");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<RendererComponent>()->Layer = 11;

	auto backBtn = btnObj->AddComponent<ButtonComponent>();
	backBtn->Dimensions = { 254, 30 };
	backBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	backBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	backBtn->SetOnClickFunction(new SwitchScene{ "MainMenuScene" });

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->SetLocation(400, 300);
	btnObj->SetTag("BackToMainBtn");

	//Quit Btn
	btnObj = menuPrefab->CreateGameObject();
	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Quit");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<RendererComponent>()->Layer = 11;

	auto quitBtn = btnObj->AddComponent<ButtonComponent>();
	quitBtn->Dimensions = { 65, 30 };
	quitBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	quitBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	quitBtn->SetOnClickFunction(new QuitGameCommand{ });

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->SetLocation(400, 400);
	btnObj->SetTag("QuitBtn");

	pPrefabManager.SavePrefab(menuPrefab, "PauseMenu");

	//Game over menu (opaque)
	auto quitMenuPrefab = pPrefabManager.CreatePrefab();
	menuObj = quitMenuPrefab->CreateGameObject();

	pShapeRenderer = menuObj->AddComponent<RendererComponent>();
	pShapeRenderer->SetShape(new geo::Rectangle{ glm::vec2{0,0},NapoleonEngine::GetEngine()->GetWindowWidth(), NapoleonEngine::GetEngine()->GetWindowHeight(), Color{0,0,0, 255}, true });
	pShapeRenderer->Layer = 10;

	textObject = quitMenuPrefab->CreateGameObject();

	textComp = textObject->AddComponent<TextRendererComponent>();
	textComp->SetText("Game Over");
	textComp->SetFont(biggerFont);

	textObject->AddComponent<RendererComponent>()->Layer = 11;
	textObject->GetTransform()->SetLocation(glm::vec2{ 400, 100 });

	menuObj->AddChild(textObject);

	btnObj = quitMenuPrefab->CreateGameObject();

	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Replay");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<RendererComponent>()->Layer = 11;
	auto replayBtn = btnObj->AddComponent<ButtonComponent>();
	replayBtn->Dimensions = { 100, 30 };
	replayBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	replayBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	replayBtn->SetOnClickFunction(new ReloadSceneCommand{});

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->SetLocation(400, 200);
	btnObj->SetTag("ReplayBtn");

	//Back to main btn
	btnObj = quitMenuPrefab->CreateGameObject();

	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Back to Main Menu");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<RendererComponent>()->Layer = 11;

	backBtn = btnObj->AddComponent<ButtonComponent>();
	backBtn->Dimensions = { 260, 30 };
	backBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	backBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	backBtn->SetOnClickFunction(new SwitchScene{ "MainMenuScene" });

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->SetLocation(400, 300);
	btnObj->SetTag("BackToMainBtn");
	
	//Quit Btn
	btnObj = quitMenuPrefab->CreateGameObject();

	textComp = btnObj->AddComponent<TextRendererComponent>();
	textComp->SetText("Quit");
	textComp->SetFont(lessBigFont);

	btnObj->AddComponent<RendererComponent>()->Layer = 11;

	quitBtn = btnObj->AddComponent<ButtonComponent>();
	quitBtn->Dimensions = { 65, 30 };
	quitBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,} });
	quitBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	quitBtn->SetOnClickFunction(new QuitGameCommand{ });

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->SetLocation(400, 400);
	btnObj->SetTag("QuitBtn");

	pPrefabManager.SavePrefab(quitMenuPrefab, "GameOverMenu");
}