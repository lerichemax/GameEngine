#include "PCH.h"
#include "VersusScene.h"

#include "CharacterLives.h"
#include "CharacterPoint.h"
#include "Coily.h"
#include "CoilyCharacterController.h"
#include "Command.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "PauseGameCommand.h"
#include "QBert.h"
#include "VersusGameManager.h"
#include "CameraComponent.h"

#include "TextRendererComponent.h"
#include "PrefabsManager.h"
#include "QuitGameCommand.h"
#include "ReloadSceneCommand.h"
#include "ResourceManager.h"
#include "SwitchScene.h"
#include "SwitchTextColor.h"

VersusScene::VersusScene() 
	:QBertScene("VersusScene"),
	m_pCoilyPlayer(nullptr),
	m_pRoundText(nullptr),
	m_pTextP1(nullptr),
	m_pTextP2(nullptr)
{
}

void VersusScene::Initialize()
{
	//auto const& pPrefabManager = PrefabsManager::Get();
	//
	//AddObject(pPrefabManager.Instantiate("FPSCounter"));

	//auto const pointsP1 = pPrefabManager.Instantiate("PointsUI");
	//m_pTextP1 = pointsP1->GetComponent<TextRendererComponent>();
	//m_pTextP1->SetText("P1: 0");
	//AddObject(pointsP1);

	//auto const pointsP2 = pPrefabManager.Instantiate("PointsUI");
	//m_pTextP2 = pointsP2->GetComponent<TextRendererComponent>();
	//m_pTextP2->SetText("P2: 0 ");
	//m_pTextP2->SetTextColor(255, 0, 255);
	//pointsP2->GetECSTransform()->Translate(20.f, 80.f);
	//AddObject(pointsP2);

	////Pause Menu
	//m_pPauseMenu = pPrefabManager.Instantiate("PauseMenu");
	//AddObject(m_pPauseMenu);

	//auto pBtnObject = m_pPauseMenu->FindTagInChildren("ResumeBtn");
	//auto pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//auto pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
	//pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	//pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new PauseGameCommand{ this, m_pPauseMenu });

	//pBtnObject = m_pPauseMenu->FindTagInChildren("BackToMainBtn");
	//pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
	//pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	//pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new SwitchScene{ "MainMenuScene" });

	//pBtnObject = m_pPauseMenu->FindTagInChildren("QuitBtn");
	//pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
	//pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	//pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new QuitGameCommand{  });

	//m_pPauseMenu->SetActive(false);

	//auto const fontBig = ResourceManager::Get().GetFont("Fonts/Lingua.otf", 24);
	//
	//auto* const roundTxt = new GameObject{};
	//roundTxt->GetECSTransform()->Translate(400.f, 60.f);
	////roundTxt->AddComponent(new TextRendererComponent{ "Round 1", fontBig });
	//roundTxt->GetComponent<TextRendererComponent>()->SetTextColor(255, 255, 255);
	//m_pRoundText = roundTxt->GetComponent<TextRendererComponent>();
	//AddObject(roundTxt);
	//
	//auto qbertObj = pPrefabManager.Instantiate("QBert");
	//m_pQbert = qbertObj->GetComponent<QBert>();
	//m_pQbert->SetPlayerNbr(1);
	//AddObject(qbertObj);
	//
	//auto pyramid = pPrefabManager.Instantiate("Pyramid");
	//m_pPyramid = pyramid->GetComponent<Pyramid>();
	//m_pPyramid->SetQBert(m_pQbert);
	//AddObject(pyramid);
	//m_pQbert->SetCurrentQube(m_pPyramid->GetTop());

	//auto coilyObj = pPrefabManager.Instantiate("Coily");
	//m_pCoilyPlayer = coilyObj->GetComponent<Coily>();
	//
	//coilyObj->AddComponent(new CharacterPoint{});
	//coilyObj->AddComponent(new CharacterLives{ 3 });
	//coilyObj->GetComponent<CoilyCharacterController>()->SetPyramid(m_pPyramid);
	//AddObject(coilyObj);
	//m_pCoilyPlayer->SetCurrentQube(m_pPyramid->GetQube(2));

	////game over menu
	//m_pGameOverMenu = pPrefabManager.Instantiate("GameOverMenu");
	//AddObject(m_pGameOverMenu);

	////Replay
	//pBtnObject = m_pGameOverMenu->FindTagInChildren("ReplayBtn");
	//pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//pTextComp = pBtn->GetGameObject()->GetComponent<TextRendererComponent>();
	//pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	//pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new ReloadSceneCommand{ this });
	////Back to main
	//pBtnObject = m_pGameOverMenu->FindTagInChildren("BackToMainBtn");
	//pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
	//pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	//pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new SwitchScene{ "MainMenuScene" });
	////quit
	//pBtnObject = m_pGameOverMenu->FindTagInChildren("QuitBtn");
	//pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
	//pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	//pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new QuitGameCommand{  });
	//m_pGameOverMenu->SetActive(false);

	//auto const pGameManager = new VersusGameManager{ m_pRoundText, m_pTextP1, m_pTextP2,
	//	qbertObj->GetComponent<CharacterPoint>(), coilyObj->GetComponent<CharacterPoint>(), m_pPyramid,
	//	m_pGameOverMenu, 3 };
	//ObserverManager::Get().AddObserver(pGameManager);
	//
	//qbertObj->AddObserver(pGameManager);
	//coilyObj->AddObserver(pGameManager);
	//pyramid->AddObserver(pGameManager);

	//auto* camObj = new GameObject{};
	//auto* const camComp = new CameraComponent{};
	//camObj->AddComponent(camComp);
	//camObj->GetECSTransform()->Translate(450, 300);
	//SetCameraActive(camComp);
	//AddObject(camObj);
}

//void VersusScene::ResetGame()
//{
//	//m_pPyramid->Reset();
//	
//	m_pQbert->Reset(true, m_pPyramid->GetTop());
//	//m_pQbert->GetGameObject()->GetComponent<RendererComponent>()->ChangeLayer(Layer::foreground);
//	m_pQbert->SetCanMove();
//	
//	m_pCoilyPlayer->Transform(false);
//	m_pCoilyPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
//	m_pCoilyPlayer->GetGameObject()->GetTransform()->Translate(m_pCoilyPlayer->GetCurrentQube()->GetCharacterPos());
//	//m_pCoilyPlayer->GetGameObject()->GetComponent<CoilyCharacterController>()->SetEnable(true);
//	//m_pCoilyPlayer->GetGameObject()->GetComponent<CharacterPoint>()->Reset();
//	
//	//m_pRoundText->SetText("Round 1");
//	//m_pTextP1->SetText("P1: 0");
//	//m_pTextP2->SetText("P2: 0");
//	SetIsPaused(false);
//	m_pPauseMenu->SetActive(false);
//	m_pGameOverMenu->SetActive(false);
//}

void VersusScene::ResetScene(Level ) //ignore level, always resets to level 1
{
	//m_pPyramid->Reset();
	//m_pQbert->Reset(false, m_pPyramid->GetTop());
	
	m_pCoilyPlayer->Transform(false);
	//m_pCoilyPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
	//m_pCoilyPlayer->GetGameObject()->GetTransform()->Translate(m_pCoilyPlayer->GetCurrentQube()->GetCharacterPos());
	//m_pCoilyPlayer->GetGameObject()->GetComponent<CoilyCharacterController>()->SetEnable(true);
}

void VersusScene::DeclareInput()
{
	//Player1
	//InputManager::Get().AddInputAction(24, new InputAction(SDLK_w, KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upRight, m_pQbert)));
	//InputManager::Get().AddInputAction(25, new InputAction{ SDLK_d , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downRight, m_pQbert) });
	//InputManager::Get().AddInputAction(26, new InputAction{ SDLK_s , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downLeft, m_pQbert) });
	//InputManager::Get().AddInputAction(27, new InputAction{ SDLK_a , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upLeft, m_pQbert) });

	//InputManager::Get().AddInputAction(28,
	//	new InputAction(ControllerButton::ButtonUp, KeyActionState::pressed,
	//		new MoveCommand(ConnectionDirection::upRight, m_pQbert), PlayerNbr::One));
	//InputManager::Get().AddInputAction(29,
	//	new InputAction{ ControllerButton::ButtonRight , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downRight, m_pQbert), PlayerNbr::One });
	//InputManager::Get().AddInputAction(30,
	//	new InputAction{ ControllerButton::ButtonDown , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downLeft, m_pQbert), PlayerNbr::One });
	//InputManager::Get().AddInputAction(31,
	//	new InputAction{ ControllerButton::ButtonLeft , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upLeft, m_pQbert), PlayerNbr::One });

	////Player2
	//InputManager::Get().AddInputAction(32, new InputAction(SDLK_UP, KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upRight, m_pCoilyPlayer)));
	//InputManager::Get().AddInputAction(33, new InputAction{ SDLK_RIGHT , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downRight, m_pCoilyPlayer) });
	//InputManager::Get().AddInputAction(34, new InputAction{ SDLK_DOWN , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downLeft, m_pCoilyPlayer) });
	//InputManager::Get().AddInputAction(35, new InputAction{ SDLK_LEFT , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upLeft, m_pCoilyPlayer) });

	//InputManager::Get().AddInputAction(36,
	//	new InputAction(ControllerButton::ButtonUp, KeyActionState::pressed,
	//		new MoveCommand(ConnectionDirection::upRight, m_pCoilyPlayer), PlayerNbr::Two));
	//InputManager::Get().AddInputAction(37,
	//	new InputAction{ ControllerButton::ButtonRight , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downRight, m_pCoilyPlayer), PlayerNbr::Two });
	//InputManager::Get().AddInputAction(38,
	//	new InputAction{ ControllerButton::ButtonDown , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downLeft, m_pCoilyPlayer), PlayerNbr::Two });
	//InputManager::Get().AddInputAction(39,
	//	new InputAction{ ControllerButton::ButtonLeft , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upLeft, m_pCoilyPlayer), PlayerNbr::Two });

	//InputManager::Get().AddInputAction(105, new InputAction{ SDLK_ESCAPE, KeyActionState::pressed,
	//new PauseGameCommand(this, m_pPauseMenu) });

	//InputManager::Get().AddInputAction(106, new InputAction{ ControllerButton::Start, KeyActionState::pressed,
	//	new PauseGameCommand(this, m_pPauseMenu), PlayerNbr::One });

	//InputManager::Get().AddInputAction(107, new InputAction{ ControllerButton::Start, KeyActionState::pressed,
	//new PauseGameCommand(this, m_pPauseMenu), PlayerNbr::Two });
}