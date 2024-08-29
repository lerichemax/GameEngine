#include "PCH.h"
#include "CoopScene.h"



#include "ButtonComponent.h"
#include "CharacterLives.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "PrefabsManager.h"
#include "QBert.h"
#include "TextRendererComponent.h"
#include "EnemyManager.h"
#include "CoilyManager.h"
#include "SlickSamManager.h"
#include "WrongWayManager.h"
#include "GameManager.h"
#include "ObserverManager.h"
#include "PauseGameCommand.h"
#include "QuitGameCommand.h"
#include "ReloadSceneCommand.h"
#include "SwitchScene.h"
#include "SwitchTextColor.h"
#include "CameraComponent.h"

CoopScene::CoopScene()
	: QBertScene("CoopScene"),
	m_pQbertP2(nullptr)
{
	
}

void CoopScene::Initialize()
{
	//auto& pPrefabManager = PrefabsManager::GetInstance();
	//AddObject(PrefabsManager::GetInstance().Instantiate("FPSCounter"));

	//auto const livesP1 = pPrefabManager.Instantiate("LivesUI");
	//AddObject(livesP1);

	//auto const pointsP1 = pPrefabManager.Instantiate("PointsUI");
	//AddObject(pointsP1);

	//auto const livesP2 = pPrefabManager.Instantiate("LivesUI");
	//livesP2->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 255);
	//livesP2->GetComponent<TextRendererComponent>()->SetText("P2 Lives: 0");
	//AddObject(livesP2);
	//livesP2->GetECSTransform()->Translate(20.f, 80.f);
	//
	//auto const pointsP2 = pPrefabManager.Instantiate("PointsUI");
	//pointsP2->GetComponent<TextRendererComponent>()->SetText("P2 Points: 0");
	//pointsP2->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 255);
	//AddObject(pointsP2);
	//pointsP2->GetECSTransform()->Translate(20.f, 100.f);

	////Pause Menu
	//m_pPauseMenu = pPrefabManager.Instantiate("PauseMenu");
	//AddObject(m_pPauseMenu);

	//auto pBtnObject = m_pPauseMenu->FindTagInChildren("ResumeBtn");
	//auto pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//auto pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
	////pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	////pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new PauseGameCommand{ this, m_pPauseMenu });

	//pBtnObject = m_pPauseMenu->FindTagInChildren("BackToMainBtn");
	//pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
	////pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	////pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new SwitchScene{ "MainMenuScene" });

	//pBtnObject = m_pPauseMenu->FindTagInChildren("QuitBtn");
	//pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
	////pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	////pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new QuitGameCommand{  });
	//m_pPauseMenu->SetActive(false);

	//auto qbert1 = PrefabsManager::GetInstance().Instantiate("QBert");
	////m_pQbert = qbert1->GetComponent<QBert>();
	//m_pQbert->SetPlayerNbr(1);
	//AddObject(qbert1);
	//
	//auto qbert2 = PrefabsManager::GetInstance().Instantiate("QBert");
	////m_pQbertP2 = qbert2->GetComponent<QBert>();
	//m_pQbertP2->SetPlayerNbr(2);
	//AddObject(qbert2);

	//livesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + 
	//	std::to_string(qbert1->GetComponent<CharacterLives>()->GetNbrLives()));
	//livesP2->GetComponent<TextRendererComponent>()->SetText("P2 Lives: " + 
	//	std::to_string(qbert2->GetComponent<CharacterLives>()->GetNbrLives()));
	//
	//auto pyramid = PrefabsManager::GetInstance().Instantiate("Pyramid");
	////m_pPyramid = pyramid->GetComponent<Pyramid>();
	//AddObject(pyramid);

	//auto* enemyManagerObj = new GameObject{};
	//auto* pWWm = new WrongWayManager{ 2, 7 };
	//auto* pSSm = new SlickSamManager{ 2, 7 };
	//pWWm->SetPyramid(m_pPyramid);
	//pSSm->SetPyramid(m_pPyramid);
	//enemyManagerObj->AddComponent(pWWm);
	//enemyManagerObj->AddComponent(pSSm);
	//m_pEnemyManagers.push_back(pWWm);
	//m_pEnemyManagers.push_back(pSSm);
	//AddObject(enemyManagerObj);
	//
	//m_pQbert->SetCurrentQube(m_pPyramid->GetEscheresqueLeftTop());
	//m_pQbertP2->SetCurrentQube(m_pPyramid->GetEscheresqueRightTop());

	////game over menu
	//m_pGameOverMenu = pPrefabManager.Instantiate("GameOverMenu");
	//AddObject(m_pGameOverMenu);

	////Replay
	//pBtnObject = m_pGameOverMenu->FindTagInChildren("ReplayBtn");
	//pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
	////pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	////pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new ReloadSceneCommand{ this });
	////Back to main
	//pBtnObject = m_pGameOverMenu->FindTagInChildren("BackToMainBtn");
	//pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
	////pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	////pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new SwitchScene{ "MainMenuScene" });
	////quit
	//pBtnObject = m_pGameOverMenu->FindTagInChildren("QuitBtn");
	//pBtn = pBtnObject->GetComponent<ButtonComponent>();
	//pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
	////pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
	////pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
	//pBtn->SetOnClickFunction(new QuitGameCommand{  });

	//m_pGameOverMenu->SetActive(false);
	//
	////auto pGameManager = new GameManager{ pointsP1->GetComponent<TextRendererComponent>(),
	////pointsP2->GetComponent<TextRendererComponent>(), livesP1->GetComponent<TextRendererComponent>(),
	////	livesP2->GetComponent<TextRendererComponent>(), nullptr, pWWm, pSSm, m_pGameOverMenu };
	////pGameManager->SetNbrPlayers(2);
	////ObserverManager::GetInstance().AddObserver(pGameManager);
	////pWWm->SetGameManager(pGameManager);
	////pSSm->SetGameManager(pGameManager);
	////
	////qbert1->AddObserver(pGameManager);
	////qbert2->AddObserver(pGameManager);

	//auto const camObj = new GameObject{};
	//auto const camComp = new CameraComponent{};
	//camObj->AddComponent(camComp);
	//camObj->GetECSTransform()->Translate(450, 300);
	//SetCameraActive(camComp);
	//AddObject(camObj);
}

void CoopScene::ResetScene(Level newLevel)
{
	m_Level = newLevel;
	//m_pPyramid->Reset();
	for (EnemyManager* pManager : m_pEnemyManagers)
	{
		pManager->Reset();
	}
	//m_pQbert->Reset(false, m_pPyramid->GetEscheresqueLeftTop());
	//m_pQbert->GetGameObject()->SetActive(true);
	//m_pQbertP2->Reset(false, m_pPyramid->GetEscheresqueRightTop());
	//m_pQbertP2->GetGameObject()->SetActive(true);
}

//void CoopScene::ResetGame()
//{
//	m_Level = Level::Level1;
//	//m_pPyramid->Reset();
//
//	for (EnemyManager* pManager : m_pEnemyManagers)
//	{
//		pManager->Reset();
//		pManager->ResetTimer();
//	}
//	
//	m_pQbert->GetGameObject()->SetActive(true);
//	m_pQbertP2->GetGameObject()->SetActive(true);
//	//m_pQbert->Reset(true, m_pPyramid->GetEscheresqueLeftTop());
//	//m_pQbertP2->Reset(true, m_pPyramid->GetEscheresqueRightTop());
//	SetIsPaused(false);
//	m_pPauseMenu->SetActive(false);
//	m_pGameOverMenu->SetActive(false);
//}

void CoopScene::DeclareInput()
{
	////Player1
	//InputManager::GetInstance().AddInputAction(8, new InputAction(SDLK_w, KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upRight, m_pQbert)));
	//InputManager::GetInstance().AddInputAction(9, new InputAction{ SDLK_d , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downRight, m_pQbert) });
	//InputManager::GetInstance().AddInputAction(10, new InputAction{ SDLK_s , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downLeft, m_pQbert) });
	//InputManager::GetInstance().AddInputAction(11, new InputAction{ SDLK_a , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upLeft, m_pQbert) });

	//InputManager::GetInstance().AddInputAction(12,
	//	new InputAction(ControllerButton::ButtonUp, KeyActionState::pressed,
	//		new MoveCommand(ConnectionDirection::upRight, m_pQbert), PlayerNbr::One));
	//InputManager::GetInstance().AddInputAction(13,
	//	new InputAction{ ControllerButton::ButtonRight , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downRight, m_pQbert), PlayerNbr::One });
	//InputManager::GetInstance().AddInputAction(14,
	//	new InputAction{ ControllerButton::ButtonDown , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downLeft, m_pQbert), PlayerNbr::One });
	//InputManager::GetInstance().AddInputAction(15,
	//	new InputAction{ ControllerButton::ButtonLeft , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upLeft, m_pQbert), PlayerNbr::One });

	////Player2
	//InputManager::GetInstance().AddInputAction(16, new InputAction(SDLK_UP, KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upRight, m_pQbertP2)));
	//InputManager::GetInstance().AddInputAction(17, new InputAction{ SDLK_RIGHT , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downRight, m_pQbertP2) });
	//InputManager::GetInstance().AddInputAction(18, new InputAction{ SDLK_DOWN , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downLeft, m_pQbertP2) });
	//InputManager::GetInstance().AddInputAction(19, new InputAction{ SDLK_LEFT , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upLeft, m_pQbertP2) });

	//InputManager::GetInstance().AddInputAction(20,
	//	new InputAction(ControllerButton::ButtonUp, KeyActionState::pressed,
	//		new MoveCommand(ConnectionDirection::upRight, m_pQbertP2), PlayerNbr::Two));
	//InputManager::GetInstance().AddInputAction(21,
	//	new InputAction{ ControllerButton::ButtonRight , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downRight, m_pQbertP2), PlayerNbr::Two });
	//InputManager::GetInstance().AddInputAction(22,
	//	new InputAction{ ControllerButton::ButtonDown , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downLeft, m_pQbertP2), PlayerNbr::Two });
	//InputManager::GetInstance().AddInputAction(23,
	//	new InputAction{ ControllerButton::ButtonLeft , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upLeft, m_pQbertP2), PlayerNbr::Two });

	//InputManager::GetInstance().AddInputAction(102, new InputAction{ SDLK_ESCAPE, KeyActionState::pressed,
	//	new PauseGameCommand(this, m_pPauseMenu) });

	//InputManager::GetInstance().AddInputAction(103, new InputAction{ ControllerButton::Start, KeyActionState::pressed,
	//	new PauseGameCommand(this, m_pPauseMenu), PlayerNbr::One });
	//
	//InputManager::GetInstance().AddInputAction(104, new InputAction{ ControllerButton::Start, KeyActionState::pressed,
	//new PauseGameCommand(this, m_pPauseMenu), PlayerNbr::Two });

}