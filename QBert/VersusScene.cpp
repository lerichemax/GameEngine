#include "PCH.h"
#include "VersusScene.h"

#include "UIManagerSystem.h"
#include "DiskSystem.h"
#include "GameManagerSystem.h"

#include "UiManagerComponent.h"
#include "CharacterLives.h"
#include "TextRendererComponent.h"
#include "ButtonComponent.h"
#include "CharacterControllerComponent.h"
#include "CharacterPoint.h"
#include "CoilyComponent.h"

#include "PauseGameCommand.h"

VersusScene::VersusScene() 
	:Scene("VersusScene")
{
}

void VersusScene::Initialize()
{
	auto pPointsP1 = Instantiate("PointsUI");

	auto pPointsP2 = Instantiate("PointsUI");

	auto pUiManagerObj = CreateGameObject();
	auto pUiComp = pUiManagerObj->AddComponent<UiManagerComponent>();
	pUiComp->PointsCounterTextEntity = pPointsP1->GetEntity();
	
	pUiComp->PointsCounterTextEntityP2 = pPointsP2->GetEntity();

	AddSystem<UIManagerSystem>();

	//Pause Menu
	auto pPauseMenuObject = Instantiate("PauseMenu");
	pPauseMenuObject->SetActive(false);
	pUiComp->PauseMenuEntity = pPauseMenuObject->GetEntity();

	//Game Over menu
	auto pGameOverMenu = Instantiate("GameOverMenu");
	pGameOverMenu->SetActive(false);
	pUiComp->GameOverMenuEntity = pGameOverMenu->GetEntity();

	//pyramid
	Instantiate("Pyramid");
	AddSystem<DiskSystem>();

	//Qbert
	auto pQbertObj = Instantiate("QBert");
	pQbertObj->GetComponent<CharacterLives>()->Init(4);

	//game manager
	auto* const pGameManager = AddSystem<GameManagerSystem>();
	pGameManager->SetGameMode(GameMode::Versus);

	pPauseMenuObject->FindChildrenWithTag("ResumeBtn")->GetComponent<ButtonComponent>()->SetOnClickFunction(new PauseGameCommand{ pGameManager });

	//Coily
	auto pCoilyObj = Instantiate("Coily");
	auto* const pCharacterController = pCoilyObj->AddComponent<CharacterControllerComponent>();
	pCharacterController->Right = SDL_SCANCODE_RIGHT;
	pCharacterController->Left = SDL_SCANCODE_LEFT;
	pCharacterController->Up = SDL_SCANCODE_UP;
	pCharacterController->Down = SDL_SCANCODE_DOWN;
	pCharacterController->PlayerNumber = 2;
	pCharacterController->SetActive(false);

	pCoilyObj->GetComponent<CharacterLives>()->Init(4);
	pCoilyObj->AddComponent<CharacterPoint>();

	pCoilyObj->GetComponent<CoilyComponent>()->SetActive(false);

	auto pTextPointsP2 = pPointsP2->GetComponent<TextRendererComponent>();
	pTextPointsP2->SetText("P2 Points: " + std::to_string(pCoilyObj->GetComponent<CharacterPoint>()->GetPoints()));
	pTextPointsP2->SetTextColor(255, 0, 255);
	pPointsP2->GetTransform()->Translate(20.f, 100.f);

	Instantiate("FPSCounter");
	GetCameraObject()->GetTransform()->Translate(450, 300);
}

//void VersusScene::ResetGame()
//{
//	//m_pPyramid->Reset();
//	
//	m_pQbert->Reset(true, m_pPyramid->GetTop());
//	//m_pQbert->GetEntity()->GetComponent<RendererComponent>()->ChangeLayer(Layer::foreground);
//	m_pQbert->SetCanMove();
//	
//	m_pCoilyPlayer->Transform(false);
//	m_pCoilyPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
//	m_pCoilyPlayer->GetEntity()->GetTransform()->Translate(m_pCoilyPlayer->GetCurrentQube()->GetCharacterPos());
//	//m_pCoilyPlayer->GetEntity()->GetComponent<CoilyCharacterController>()->SetEnable(true);
//	//m_pCoilyPlayer->GetEntity()->GetComponent<CharacterPoint>()->Reset();
//	
//	//m_pRoundText->SetText("Round 1");
//	//m_pTextP1->SetText("P1: 0");
//	//m_pTextP2->SetText("P2: 0");
//	SetIsPaused(false);
//	m_pPauseMenu->SetActive(false);
//	m_pGameOverMenu->SetActive(false);
//}

//void VersusScene::ResetScene(Level ) //ignore level, always resets to level 1
//{
	//m_pPyramid->Reset();
	//m_pQbert->Reset(false, m_pPyramid->GetTop());
	
	//m_pCoilyPlayer->Transform(false);
	//m_pCoilyPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
	//m_pCoilyPlayer->GetEntity()->GetTransform()->Translate(m_pCoilyPlayer->GetCurrentQube()->GetCharacterPos());
	//m_pCoilyPlayer->GetEntity()->GetComponent<CoilyCharacterController>()->SetEnable(true);
//}

//void VersusScene::DeclareInput()
//{
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
//}