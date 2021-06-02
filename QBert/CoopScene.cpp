#include "PCH.h"
#include "CoopScene.h"



#include "ButtonComponent.h"
#include "CharacterLives.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "Pyramid.h"
#include "PrefabsManager.h"
#include "QBert.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"
#include "EnemyManager.h"
#include "GameManager.h"
#include "ObserverManager.h"
#include "PauseGameCommand.h"
#include "QuitGameCommand.h"
#include "ReloadSceneCommand.h"
#include "SwitchScene.h"
#include "SwitchTextColor.h"

using namespace empire;
CoopScene::CoopScene() : QBertScene("CoopScene")
{
	
}

void CoopScene::Initialize()
{
	auto& pPrefabManager = empire::PrefabsManager::GetInstance();
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

	auto qbert1 = PrefabsManager::GetInstance().Instantiate("QBert");
	m_pQbert = qbert1->GetComponent<QBert>();
	m_pQbert->SetPlayerNbr(1);
	AddObject(qbert1);
	
	auto qbert2 = PrefabsManager::GetInstance().Instantiate("QBert");
	m_pQbertP2 = qbert2->GetComponent<QBert>();
	m_pQbertP2->SetPlayerNbr(2);
	AddObject(qbert2);

	livesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + 
		std::to_string(qbert1->GetComponent<CharacterLives>()->GetNbrLives()));
	livesP2->GetComponent<TextRendererComponent>()->SetText("P2 Lives: " + 
		std::to_string(qbert2->GetComponent<CharacterLives>()->GetNbrLives()));
	
	auto pyramid = PrefabsManager::GetInstance().Instantiate("Pyramid");
	m_pPyramid = pyramid->GetComponent<Pyramid>();
	AddObject(pyramid);

	auto enemyManagerObj = new GameObject{};
	m_pEnemyManager = new EnemyManager{};
	m_pEnemyManager->SetPyramid(m_pPyramid);
	enemyManagerObj->AddComponent(m_pEnemyManager);
	AddObject(enemyManagerObj);

	m_pQbert->SetCurrentQube(m_pPyramid->GetEscheresqueLeftTop());
	m_pQbertP2->SetCurrentQube(m_pPyramid->GetEscheresqueRightTop());

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
	pointsP2->GetComponent<TextRendererComponent>(), livesP1->GetComponent<TextRendererComponent>(),
		livesP2->GetComponent<TextRendererComponent>(), m_pEnemyManager, m_pGameOverMenu };
	pGameManager->SetNbrPlayers(2);
	ObserverManager::GetInstance().AddObserver(pGameManager);
	qbert1->AddObserver(pGameManager);
	qbert2->AddObserver(pGameManager);
}

void CoopScene::ResetScene(Level newLevel)
{
	m_Level = newLevel;
	m_pPyramid->Reset();
	m_pEnemyManager->Reset();
	m_pQbert->Reset(false, m_pPyramid->GetEscheresqueLeftTop());
	m_pQbert->GetGameObject()->SetActive(true);
	m_pQbertP2->Reset(false, m_pPyramid->GetEscheresqueRightTop());
	m_pQbertP2->GetGameObject()->SetActive(true);
}

void CoopScene::ResetGame()
{
	m_Level = Level::Level1;
	m_pPyramid->Reset();
	m_pEnemyManager->Reset();
	m_pQbert->GetGameObject()->SetActive(true);
	m_pQbertP2->GetGameObject()->SetActive(true);
	m_pQbert->Reset(true, m_pPyramid->GetEscheresqueLeftTop());
	m_pQbertP2->Reset(true, m_pPyramid->GetEscheresqueRightTop());
	m_pPauseMenu->SetActive(false);
	m_pGameOverMenu->SetActive(false);
	
}

void CoopScene::DeclareInput()
{
	//Player1
	InputManager::GetInstance().AddInputAction(8, new InputAction(SDLK_w, empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upRight, m_pQbert)));
	InputManager::GetInstance().AddInputAction(9, new InputAction{ SDLK_d , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pQbert) });
	InputManager::GetInstance().AddInputAction(10, new InputAction{ SDLK_s , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pQbert) });
	InputManager::GetInstance().AddInputAction(11, new InputAction{ SDLK_a , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pQbert) });

	InputManager::GetInstance().AddInputAction(12,
		new InputAction(ControllerButton::ButtonUp, empire::KeyActionState::pressed,
			new MoveCommand(ConnectionDirection::upRight, m_pQbert), PlayerNbr::One));
	InputManager::GetInstance().AddInputAction(13,
		new InputAction{ ControllerButton::ButtonRight , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pQbert), PlayerNbr::One });
	InputManager::GetInstance().AddInputAction(14,
		new InputAction{ ControllerButton::ButtonDown , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pQbert), PlayerNbr::One });
	InputManager::GetInstance().AddInputAction(15,
		new InputAction{ ControllerButton::ButtonLeft , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pQbert), PlayerNbr::One });

	//Player2
	InputManager::GetInstance().AddInputAction(16, new InputAction(SDLK_UP, empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upRight, m_pQbertP2)));
	InputManager::GetInstance().AddInputAction(17, new InputAction{ SDLK_RIGHT , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pQbertP2) });
	InputManager::GetInstance().AddInputAction(18, new InputAction{ SDLK_DOWN , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pQbertP2) });
	InputManager::GetInstance().AddInputAction(19, new InputAction{ SDLK_LEFT , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pQbertP2) });

	InputManager::GetInstance().AddInputAction(20,
		new InputAction(ControllerButton::ButtonUp, empire::KeyActionState::pressed,
			new MoveCommand(ConnectionDirection::upRight, m_pQbertP2), PlayerNbr::Two));
	InputManager::GetInstance().AddInputAction(21,
		new InputAction{ ControllerButton::ButtonRight , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pQbertP2), PlayerNbr::Two });
	InputManager::GetInstance().AddInputAction(22,
		new InputAction{ ControllerButton::ButtonDown , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pQbertP2), PlayerNbr::Two });
	InputManager::GetInstance().AddInputAction(23,
		new InputAction{ ControllerButton::ButtonLeft , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pQbertP2), PlayerNbr::Two });

	InputManager::GetInstance().AddInputAction(102, new InputAction{ ControllerButton::Start, empire::KeyActionState::pressed,
	new PauseGameCommand(this, m_pPauseMenu), PlayerNbr::Two });
}