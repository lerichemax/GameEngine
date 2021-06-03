#include "PCH.h"
#include "VersusScene.h"


#include "ButtonComponent.h"
#include "Pyramid.h"
#include "CharacterLives.h"
#include "CharacterPoint.h"
#include "Coily.h"
#include "CoilyCharacterController.h"
#include "Command.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "ObserverManager.h"
#include "PauseGameCommand.h"
#include "QBert.h"
#include "VersusGameManager.h"

#include "TextRendererComponent.h"
#include "PrefabsManager.h"
#include "QuitGameCommand.h"
#include "ReloadSceneCommand.h"
#include "ResourceManager.h"
#include "SwitchScene.h"
#include "SwitchTextColor.h"


using namespace empire;

VersusScene::VersusScene() : QBertScene("VersusScene")
{
}

void VersusScene::Initialize()
{
	auto const& pPrefabManager = PrefabsManager::GetInstance();
	
	AddObject(pPrefabManager.Instantiate("FPSCounter"));

	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 15);
	
	auto const pointsP1 = new GameObject{};
	pointsP1->GetTransform()->Translate(20.f, 60.f);
	pointsP1->AddComponent(new TextRendererComponent{ "P1: 0 ", font });
	pointsP1->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	m_pTextP1 = pointsP1->GetComponent<TextRendererComponent>();
	AddObject(pointsP1);
	
	auto const pointsP2 = new GameObject{};
	pointsP2->GetTransform()->Translate(20.f, 80.f);
	pointsP2->AddComponent(new TextRendererComponent{ "P2: 0 ", font });
	pointsP2->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 255);
	m_pTextP2 = pointsP2->GetComponent<TextRendererComponent>();
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

	auto const fontBig = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 24);
	
	auto const roundTxt = new GameObject{};
	roundTxt->GetTransform()->Translate(400.f, 60.f);
	roundTxt->AddComponent(new TextRendererComponent{ "Round 1", fontBig });
	roundTxt->GetComponent<TextRendererComponent>()->SetTextColor(255, 255, 255);
	m_pRoundText = roundTxt->GetComponent<TextRendererComponent>();
	AddObject(roundTxt);
	
	auto qbertObj = pPrefabManager.Instantiate("QBert");
	m_pQbert = qbertObj->GetComponent<QBert>();
	m_pQbert->SetPlayerNbr(1);
	AddObject(qbertObj);
	
	auto pyramid = pPrefabManager.Instantiate("Pyramid");
	m_pPyramid = pyramid->GetComponent<Pyramid>();
	m_pPyramid->SetQBert(m_pQbert);
	AddObject(pyramid);
	m_pQbert->SetCurrentQube(m_pPyramid->GetTop());

	auto coilyObj = pPrefabManager.Instantiate("Coily");
	m_pCoilyPlayer = coilyObj->GetComponent<Coily>();
	m_pCoilyPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
	coilyObj->AddComponent(new CharacterPoint{});
	coilyObj->AddComponent(new CharacterLives{ 3 });
	coilyObj->GetComponent<CoilyCharacterController>()->SetPyramid(m_pPyramid);
	AddObject(coilyObj);

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

	auto pGameManager = new VersusGameManager{ m_pRoundText, m_pTextP1, m_pTextP2,
		qbertObj->GetComponent<CharacterPoint>(), coilyObj->GetComponent<CharacterPoint>(), m_pPyramid,
		m_pGameOverMenu, 3 };
	ObserverManager::GetInstance().AddObserver(pGameManager);
	
	qbertObj->AddObserver(pGameManager);
	coilyObj->AddObserver(pGameManager);
	pyramid->AddObserver(pGameManager);

	

}

void VersusScene::ResetGame()
{
	m_pPyramid->Reset();
	
	m_pQbert->Reset(true, m_pPyramid->GetTop());
	m_pQbert->GetGameObject()->GetComponent<empire::RendererComponent>()->ChangeLayer(empire::Layer::foreground);
	m_pQbert->SetCanMove();
	
	m_pCoilyPlayer->Transform(false);
	m_pCoilyPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
	m_pCoilyPlayer->GetGameObject()->GetTransform()->Translate(m_pCoilyPlayer->GetCurrentQube()->GetCharacterPos());
	m_pCoilyPlayer->GetGameObject()->GetComponent<CoilyCharacterController>()->SetEnable(true);
	m_pCoilyPlayer->GetGameObject()->GetComponent<CharacterPoint>()->Reset();
	
	m_pRoundText->SetText("Round 1");
	m_pTextP1->SetText("P1: 0");
	m_pTextP2->SetText("P2: 0");

	m_pPauseMenu->SetActive(false);
	m_pGameOverMenu->SetActive(false);
}

void VersusScene::ResetScene(Level ) //ignore level, always resets to level 1
{
	m_pPyramid->Reset();
	m_pQbert->Reset(false, m_pPyramid->GetTop());
	
	m_pCoilyPlayer->Transform(false);
	m_pCoilyPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
	m_pCoilyPlayer->GetGameObject()->GetTransform()->Translate(m_pCoilyPlayer->GetCurrentQube()->GetCharacterPos());
	m_pCoilyPlayer->GetGameObject()->GetComponent<CoilyCharacterController>()->SetEnable(true);
}

void VersusScene::DeclareInput()
{
	//Player1
	InputManager::GetInstance().AddInputAction(24, new InputAction(SDLK_w, empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upRight, m_pQbert)));
	InputManager::GetInstance().AddInputAction(25, new InputAction{ SDLK_d , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pQbert) });
	InputManager::GetInstance().AddInputAction(26, new InputAction{ SDLK_s , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pQbert) });
	InputManager::GetInstance().AddInputAction(27, new InputAction{ SDLK_a , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pQbert) });

	InputManager::GetInstance().AddInputAction(28,
		new InputAction(ControllerButton::ButtonUp, empire::KeyActionState::pressed,
			new MoveCommand(ConnectionDirection::upRight, m_pQbert), PlayerNbr::One));
	InputManager::GetInstance().AddInputAction(29,
		new InputAction{ ControllerButton::ButtonRight , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pQbert), PlayerNbr::One });
	InputManager::GetInstance().AddInputAction(30,
		new InputAction{ ControllerButton::ButtonDown , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pQbert), PlayerNbr::One });
	InputManager::GetInstance().AddInputAction(31,
		new InputAction{ ControllerButton::ButtonLeft , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pQbert), PlayerNbr::One });

	//Player2
	InputManager::GetInstance().AddInputAction(32, new InputAction(SDLK_UP, empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upRight, m_pCoilyPlayer)));
	InputManager::GetInstance().AddInputAction(33, new InputAction{ SDLK_RIGHT , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pCoilyPlayer) });
	InputManager::GetInstance().AddInputAction(34, new InputAction{ SDLK_DOWN , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pCoilyPlayer) });
	InputManager::GetInstance().AddInputAction(35, new InputAction{ SDLK_LEFT , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pCoilyPlayer) });

	InputManager::GetInstance().AddInputAction(36,
		new InputAction(ControllerButton::ButtonUp, empire::KeyActionState::pressed,
			new MoveCommand(ConnectionDirection::upRight, m_pCoilyPlayer), PlayerNbr::Two));
	InputManager::GetInstance().AddInputAction(37,
		new InputAction{ ControllerButton::ButtonRight , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pCoilyPlayer), PlayerNbr::Two });
	InputManager::GetInstance().AddInputAction(38,
		new InputAction{ ControllerButton::ButtonDown , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pCoilyPlayer), PlayerNbr::Two });
	InputManager::GetInstance().AddInputAction(39,
		new InputAction{ ControllerButton::ButtonLeft , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pCoilyPlayer), PlayerNbr::Two });

	InputManager::GetInstance().AddInputAction(105, new InputAction{ SDLK_ESCAPE, empire::KeyActionState::pressed,
	new PauseGameCommand(this, m_pPauseMenu) });

	InputManager::GetInstance().AddInputAction(106, new InputAction{ ControllerButton::Start, empire::KeyActionState::pressed,
		new PauseGameCommand(this, m_pPauseMenu), PlayerNbr::One });

	InputManager::GetInstance().AddInputAction(107, new InputAction{ ControllerButton::Start, empire::KeyActionState::pressed,
	new PauseGameCommand(this, m_pPauseMenu), PlayerNbr::Two });
}