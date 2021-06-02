#include "PCH.h"
#include "SoloScene.h"

#include "Pyramid.h"
#include "QBert.h"
#include "MoveCommand.h"
#include "EnemyManager.h"
#include "PauseGameCommand.h"
#include "GameManager.h"

#include "ResourceManager.h"
#include "InputManager.h"
#include "RendererComponent.h"
#include "ObserverManager.h"
#include "PrefabsManager.h"
#include "TextRendererComponent.h"
#include "ButtonComponent.h"
#include "CharacterController.h"
#include "CharacterLives.h"

#include "JsonReaderWriter.h"
#include "QuitGameCommand.h"
#include "ReloadSceneCommand.h"
#include "SwitchScene.h"
#include "SwitchTextColor.h"
#include "Timer.h"
#include "../3rdParty/imgui/imgui.h"

SoloScene::SoloScene()
	:QBertScene("SoloScene")
{
}

void SoloScene::Initialize()
{
	auto const& pPrefabManager = PrefabsManager::GetInstance();
	
	AddObject(pPrefabManager.Instantiate("FPSCounter"));

	auto const livesP1 = pPrefabManager.Instantiate("LivesUI");
	AddObject(livesP1);

	auto const pointsP1 = pPrefabManager.Instantiate("PointsUI");
	AddObject(pointsP1);

	//Pause Menu
	auto const lessBigFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 30);
	m_pPauseMenu = pPrefabManager.Instantiate("PauseMenu");
	AddObject(m_pPauseMenu);

	auto btnObj = new GameObject{};
	auto textComp = new TextRendererComponent{ "Resume", lessBigFont };
	textComp->ChangeLayer(Layer::uiMenuFg);
	auto btn = new ButtonComponent{ 150, 50 };
	btn->SetVisualize(true);
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, textComp });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, textComp });
	btn->SetOnClickFunction(new PauseGameCommand{ this, m_pPauseMenu });
	btnObj->AddComponent(textComp);
	btnObj->AddComponent(btn);
	m_pPauseMenu->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 200);

	//Back to main btn
	btnObj = new GameObject{};
	textComp = new TextRendererComponent{ "Back to Main Menu", lessBigFont };
	textComp->ChangeLayer(Layer::uiMenuFg);
	btn = new ButtonComponent{ 150, 50 };
	btn->SetVisualize(true);
	btnObj->AddComponent(textComp);
	btnObj->AddComponent(btn);
	btnObj->SetTag("BackToMainBtn", false);
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, textComp });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, textComp });
	btn->SetOnClickFunction(new SwitchScene{ "MainMenuScene" });
	m_pPauseMenu->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 300);

	//Quit Btn
	btnObj = new GameObject{};
	textComp = new TextRendererComponent{ "Quit", lessBigFont };
	textComp->ChangeLayer(Layer::uiMenuFg);
	btn = new ButtonComponent{ 150, 50 };
	btn->SetVisualize(true);
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, textComp });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, textComp });
	btn->SetOnClickFunction(new QuitGameCommand{});
	btnObj->AddComponent(textComp);
	btnObj->AddComponent(btn);
	btnObj->SetTag("QuitBtn", false);
	m_pPauseMenu->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 400);
	m_pPauseMenu->SetActive(false);
	
	auto qbertObj = pPrefabManager.Instantiate("QBert");
	m_pQbert = qbertObj->GetComponent<QBert>();
	m_pQbert->SetPlayerNbr(1);
	qbertObj->AddComponent(new CharacterController{});
	AddObject(qbertObj);

	livesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + 
		std::to_string(qbertObj->GetComponent<CharacterLives>()->GetNbrLives()));
	
	auto pyramid = pPrefabManager.Instantiate("Pyramid");
	m_pPyramid = pyramid->GetComponent<Pyramid>();
	m_pPyramid->SetQBert(m_pQbert);
	AddObject(pyramid);
	m_pQbert->SetCurrentQube(m_pPyramid->GetTop());
	
	auto enemyManagerObj = new GameObject{};
	m_pEnemyManager = new EnemyManager{};
	m_pEnemyManager->SetPyramid(m_pPyramid);
	enemyManagerObj->AddComponent(m_pEnemyManager);	
	
	AddObject(enemyManagerObj);

	//game over menu
	m_pGameOverMenu = pPrefabManager.Instantiate("GameOverMenu");
	AddObject(m_pGameOverMenu);
	
	//Replay
	btnObj = new GameObject{};
	textComp = new TextRendererComponent{ "Replay", lessBigFont };
	textComp->ChangeLayer(Layer::uiMenuFg);
	btn = new ButtonComponent{ 150, 50 };
	btn->SetVisualize(true);
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, textComp });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, textComp });
	btn->SetOnClickFunction(new ReloadSceneCommand{ this });
	btnObj->AddComponent(textComp);
	btnObj->AddComponent(btn);
	m_pGameOverMenu->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 200);

	//Back to main btn
	btnObj = new GameObject{};
	textComp = new TextRendererComponent{ "Back to Main Menu", lessBigFont };
	textComp->ChangeLayer(Layer::uiMenuFg);
	btn = new ButtonComponent{ 150, 50 };
	btn->SetVisualize(true);
	btnObj->AddComponent(textComp);
	btnObj->AddComponent(btn);
	btnObj->SetTag("BackToMainBtn", false);
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, textComp });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, textComp });
	btn->SetOnClickFunction(new SwitchScene{ "MainMenuScene" });
	m_pGameOverMenu->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 300);
	
	//Quit Btn
	btnObj = new GameObject{};
	textComp = new TextRendererComponent{ "Quit", lessBigFont };
	textComp->ChangeLayer(Layer::uiMenuFg);
	btn = new ButtonComponent{ 150, 50 };
	btn->SetVisualize(true);
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, textComp });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, textComp });
	btn->SetOnClickFunction(new QuitGameCommand{});
	btnObj->AddComponent(textComp);
	btnObj->AddComponent(btn);
	btnObj->SetTag("QuitBtn", false);
	m_pGameOverMenu->AddChild(btnObj);
	btnObj->GetTransform()->Translate(400, 400);
	m_pGameOverMenu->SetActive(false);

	auto pGameManager = new GameManager{ pointsP1->GetComponent<TextRendererComponent>(),
nullptr, livesP1->GetComponent<TextRendererComponent>(), nullptr, m_pEnemyManager, m_pGameOverMenu };

	ObserverManager::GetInstance().AddObserver(pGameManager);
	qbertObj->AddObserver(pGameManager);
}

void SoloScene::ResetScene(Level newLevel)
{
	m_Level = newLevel;
	m_pPyramid->Reset();
	m_pEnemyManager->Reset();
	m_pQbert->Reset(false, m_pPyramid->GetTop());
	SetIsPaused(false);
}

void SoloScene::ResetGame()
{
	m_Level = Level::Level1;
	m_pPyramid->Reset();
	m_pEnemyManager->Reset();
	m_pEnemyManager->ResetTimers();
	m_pQbert->Reset(true, m_pPyramid->GetTop());
	m_pPauseMenu->SetActive(false);
	m_pGameOverMenu->SetActive(false);
}

void SoloScene::DeclareInput()
{
	InputManager::GetInstance().AddInputAction(0, new InputAction(SDLK_w, empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upRight, m_pQbert)));
	InputManager::GetInstance().AddInputAction(1, new InputAction{ SDLK_d , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pQbert) });
	InputManager::GetInstance().AddInputAction(2, new InputAction{ SDLK_s , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pQbert) });
	InputManager::GetInstance().AddInputAction(3, new InputAction{ SDLK_a , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pQbert) });

	InputManager::GetInstance().AddInputAction(4,
		new InputAction(ControllerButton::ButtonUp, empire::KeyActionState::pressed,
			new MoveCommand(ConnectionDirection::upRight, m_pQbert), PlayerNbr::One));
	InputManager::GetInstance().AddInputAction(5,
		new InputAction{ ControllerButton::ButtonRight , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pQbert), PlayerNbr::One });
	InputManager::GetInstance().AddInputAction(6,
		new InputAction{ ControllerButton::ButtonDown , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pQbert), PlayerNbr::One });
	InputManager::GetInstance().AddInputAction(7,
		new InputAction{ ControllerButton::ButtonLeft , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pQbert), PlayerNbr::One });
}