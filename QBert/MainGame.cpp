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
#include "QuitGameCommand.h"
#include "PauseGameCommand.h"
#include "SwitchTextColor.h"
#include "SwitchScene.h"

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
	SceneManager::GetInstance().SetSceneActive("MainMenuScene");
}

void MainGame::CreatePrefabs() const
{
	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 15);

	auto& pPrefabManager = PrefabsManager::GetInstance();

	//*auto snakeFallId = */ss->AddEffect("Data/Sounds/snake-fall.mp3");

	ECS_RendererComponent rendererComp;
	rendererComp.m_Layer = Layer::uiGame;
	
	//lives
	auto const livesPrefab = pPrefabManager.CreatePrefab();
	auto livesObj = livesPrefab->CreateGameObject();
	livesObj->GetTransform()->Translate(20.f, 40.f);

	ECS_TextRendererComponent textRenderer{ "P1 Lives: ", font };
	textRenderer.SetTextColor(255, 0, 0);

	livesObj->AddComponent<ECS_TextRendererComponent>(textRenderer);
	livesObj->AddComponent<ECS_RendererComponent>(rendererComp);

	pPrefabManager.SavePrefab(livesPrefab, "LivesUI");

	//points
	auto const pointsrefab = pPrefabManager.CreatePrefab();
	auto const pointsObj = pointsrefab->CreateGameObject();
	pointsObj->GetTransform()->Translate(20.f, 60.f);
	
	textRenderer.m_Text = "P1 Points: 0 ";
	pointsObj->AddComponent<ECS_TextRendererComponent>(textRenderer);
	pointsObj->AddComponent<ECS_RendererComponent>(rendererComp);
	pPrefabManager.SavePrefab(pointsrefab, "PointsUI");

	//QBert
	auto qbertPrefab = pPrefabManager.CreatePrefab();
	auto qbert = qbertPrefab->CreateGameObject();
	rendererComp.m_Layer = Layer::foreground;
	qbert->AddComponent<ECS_RendererComponent>(rendererComp);
	qbert->AddComponent(new QBert(jumpSoundid, fallsoundId, swearSoundId));
	qbert->AddComponent<AudioComponent>(AudioComponent{ ResourceManager::GetInstance().GetEffect("Sounds/jump.mp3"), 50});
	qbert->AddComponent<AudioComponent>(AudioComponent{ ResourceManager::GetInstance().GetEffect("Sounds/swear.mp3"), 50});
	qbert->AddComponent<AudioComponent>(AudioComponent{ ResourceManager::GetInstance().GetEffect("Sounds/fall.mp3"), 50});
	qbert->AddComponent<ECS_CharacterLives>(ECS_CharacterLives{ 3 });
	qbert->AddComponent<ECS_CharacterPoint>(ECS_CharacterPoint{});
	//qbert->AddComponent(new Jumper{});
	//qbert->AddComponent(new BoxCollider{ 24,24 });
	auto hurtTextObj = qbertPrefab->CreateGameObject();
	ECS_RendererComponent hurtRenderer{};
	hurtRenderer.m_pTexture = ResourceManager::GetInstance().GetTexture("Textures/QBert/HurtText.png");
	hurtRenderer.m_Layer = Layer::foreground;
	hurtRenderer.SetActive(false);
	hurtTextObj->AddComponent<ECS_RendererComponent>(hurtRenderer);
	qbert->AddChild(hurtTextObj);
	hurtTextObj->GetTransform()->Translate(10, -20);
	qbert->GetTransform()->Scale(1.5f);
	pPrefabManager.SavePrefab(qbertPrefab, "QBert");

	//JsonReaderWriter* json = new JsonReaderWriter{ "./Data/Levels.json" };
	//
	//Qube prefab
	ECS_RendererComponent qubeRenderer;
	qubeRenderer.m_Layer = Layer::uiGame;

	auto qubePf = pPrefabManager.CreatePrefab();
	auto qubeObject = qubePf->CreateGameObject();
	auto text = ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube.png");
	//auto interText = ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube_Intermediate.png");
	//auto flippedText = ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube_Flipped.png");
	qubeObject->GetTransform()->Scale(1.75f);
	qubeRenderer.m_pTexture = text;
	qubeObject->AddComponent<ECS_RendererComponent>(qubeRenderer);
	//qubePf->AddComponent(new Qube{ text, interText, flippedText });
	pPrefabManager.SavePrefab(qubePf, "Qube");

	//Pyramid
	//int levelWidth = json->ReadInt("width");
	auto pyramidPf = pPrefabManager.CreatePrefab();
	auto pyramidObject = pyramidPf->CreateGameObject();
	pyramidObject->GetTransform()->Translate(250.f, 400.f);
	//pyramid->AddComponent(new Pyramid{ (unsigned int)levelWidth});
	//pyramidObject->AddComponent(new Pyramid{ 7});
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
	//
	//Pause Menu
	auto const biggerFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 42);
	auto const lessBigFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 30);
	auto menuPrefab = pPrefabManager.CreatePrefab();
	auto menuObj = menuPrefab->GetRoot();
	//menuObj->AddComponent(new ShapeRenderer{ +
	//	new geo::Rectangle{glm::vec2{0,0},Renderer::GetInstance().GetWindowWidth(), Renderer::GetInstance().GetWindowHeight(), Color{0,0,0, 127}, true} });

	auto textObject = menuPrefab->CreateGameObject();
	auto textComp = ECS_TextRendererComponent{ "Pause", biggerFont };
	ECS_RendererComponent menuRenderer;
	menuRenderer.m_Layer = Layer::uiMenuFg;

	textObject->AddComponent<ECS_TextRendererComponent>(textComp);
	textObject->AddComponent<ECS_RendererComponent>(menuRenderer);
	menuObj->AddChild(textObject);
	textObject->GetTransform()->Translate(glm::vec2{ 400, 100 });

	auto btnObj = menuPrefab->CreateGameObject();
	textComp = ECS_TextRendererComponent{ "Resume", lessBigFont };
	btnObj->AddComponent<ECS_TextRendererComponent>(textComp);
	btnObj->AddComponent<ECS_RendererComponent>(menuRenderer);
	ECS_ButtonComponent resumeBtn{ 110, 30 };

	resumeBtn.SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	resumeBtn.SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	//resumeBtn.SetOnClickFunction(new PauseGameCommand{this, m_pPauseMenu});

	btnObj->AddComponent<ECS_ButtonComponent>(resumeBtn);
	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 200);
	btnObj->SetTag("ResumeBtn");

	//Back to main btn
	btnObj = menuPrefab->CreateGameObject();
	textComp = ECS_TextRendererComponent{ "Back to Main Menu", lessBigFont };
	btnObj->AddComponent<ECS_TextRendererComponent>(textComp);
	btnObj->AddComponent<ECS_RendererComponent>(menuRenderer);

	ECS_ButtonComponent backBtn{ 260, 30 };
	backBtn.SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	backBtn.SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	backBtn.SetOnClickFunction(new SwitchScene{ "MainMenuScene" });
	btnObj->AddComponent<ECS_ButtonComponent>(backBtn);
	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 300);
	btnObj->SetTag("BackToMainBtn");
	
	//Quit Btn
	btnObj = menuPrefab->CreateGameObject();
	textComp = ECS_TextRendererComponent{ "Quit", lessBigFont };

	btnObj->AddComponent<ECS_TextRendererComponent>(textComp);
	btnObj->AddComponent<ECS_RendererComponent>(menuRenderer);

	ECS_ButtonComponent quitBtn{ 65, 30 };
	quitBtn.SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	quitBtn.SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	quitBtn.SetOnClickFunction(new QuitGameCommand{ });
	btnObj->AddComponent<ECS_ButtonComponent>(quitBtn);

	menuObj->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 400);
	btnObj->SetTag("QuitBtn");

	pPrefabManager.SavePrefab(menuPrefab, "PauseMenu");

	//
	////Game over menu (opaque)
	//auto quitMenuPrefab = pPrefabManager.CreatePrefab("GameOverMenu");
	//menuObj = menuPrefab->CreateGameObject();
	////menuObj->AddComponent(new ShapeRenderer{ +
	////	new geo::Rectangle{glm::vec2{0,0},Renderer::GetInstance().GetWindowWidth(), Renderer::GetInstance().GetWindowHeight(), Color{0,0,0, 255}, true} });

	//textObject = quitMenuPrefab->CreateGameObject();
	//textComp = ECS_TextRendererComponent{ "Game Over", biggerFont };
	//textObject->AddComponent<ECS_TextRendererComponent>(textComp);
	//textObject->AddComponent<ECS_RendererComponent>(menuRenderer);
	//menuObj->AddChild(textObject);
	//textObject->GetTransform()->Translate(glm::vec2{ 400, 100 });

	//btnObj = quitMenuPrefab->CreateGameObject();
	//textComp = ECS_TextRendererComponent{ "Replay", lessBigFont };
	//ECS_ButtonComponent replayBtn{ 100, 30 };
	//btnObj->AddComponent<ECS_TextRendererComponent>(textComp);
	//btnObj->AddComponent<ECS_RendererComponent>(menuRenderer);
	//
	////replayBtn.SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	////replayBtn.SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	//btnObj->AddComponent<ECS_ButtonComponent>(replayBtn);

	//menuObj->AddChild(btnObj);
	//btnObj->GetTransform()->Translate(400, 200);
	//btnObj->SetTag("ReplayBtn");

	////Back to main btn
	//btnObj = quitMenuPrefab->CreateGameObject();
	//textComp = ECS_TextRendererComponent{ "Back to Main Menu", lessBigFont };
	//btnObj->AddComponent<ECS_TextRendererComponent>(textComp);
	//btnObj->AddComponent<ECS_RendererComponent>(menuRenderer);

	//ECS_ButtonComponent backbtn{ 260, 30 };
	//backbtn.SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	//backbtn.SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	//btnObj->AddComponent<ECS_ButtonComponent>(backbtn);

	//menuObj->AddChild(btnObj);
	//btnObj->GetTransform()->Translate(400, 300);
	//btnObj->SetTag("BackToMainBtn");
	//
	////Quit Btn
	//btnObj = quitMenuPrefab->CreateGameObject();
	//textComp = ECS_TextRendererComponent{ "Quit", lessBigFont };
	//btnObj->AddComponent<ECS_TextRendererComponent>(textComp);
	//btnObj->AddComponent<ECS_RendererComponent>(menuRenderer);

	//ECS_ButtonComponent quitbtn{ 65, 30 };
	//quitbtn.SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	//quitbtn.SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, btnObj->GetComponent<ECS_TextRendererComponent>() });
	//btnObj->AddComponent<ECS_ButtonComponent>(quitbtn);

	//menuObj->AddChild(btnObj);
	//btnObj->GetTransform()->Translate(400, 400);
	//btnObj->SetTag("QuitBtn");

	//delete json;
}