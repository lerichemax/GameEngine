#include "PCH.h"
#include "SoloScene.h"

#include "PyramidSystem.h"
#include "QBertSystem.h"
#include "QubeSystem.h"

#include "QBert.h"
#include "MoveCommand.h"

#include "PauseGameCommand.h"
#include "GameManager.h"
#include "SlickSamManager.h"
#include "WrongWayManager.h"
#include "CoilyManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "CameraComponent.h"
#include "ObserverManager.h"
#include "PrefabsManager.h"
#include "TextRendererComponent.h"
#include "ButtonComponent.h"
#include "CharacterLives.h"
#include "UiSystem.h"
#include "UIManager.h"

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
	//auto const livesP1 = Instantiate("LivesUI");
	//auto const pointsP1 = Instantiate("PointsUI");

	//auto uiManagerObj = CreateGameObject();
	//auto uiManager = uiManagerObj->AddComponent<UIManager>();
	//uiManager->SetP1LivesCounter(livesP1->GetComponent<TextRendererComponent>());
	//uiManager->SetP1PointsCounter(pointsP1->GetComponent<TextRendererComponent>());

	////Pause Menu
	//auto pauseMenuObject = Instantiate("PauseMenu");
	//pauseMenuObject->SetActive(false);
	//uiManager->SetPauseMenu(pauseMenuObject);

	////Game Over menu
	//auto gameOverMenu = Instantiate("GameOverMenu");
	//gameOverMenu->SetActive(false);
	//uiManager->SetGameOverMenu(gameOverMenu);


	//pyramid
	Instantiate("Pyramid");
	AddSystem<PyramidSystem>();
	AddSystem<QubeSystem>();

	//Qbert
	Instantiate("QBert");
	//auto qbert = qbertObj->GetComponent<QBert>();
	//qbert->SetPlayerNbr(1);

	AddSystem<QBertSystem>();

	//livesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + std::to_string(qbertObj->GetComponent<CharacterLives>()->GetNbrLives()));

	//auto pyramid = pyramidObj->GetComponent<Pyramid>();
	//m_pPyramid->SetQBert(qbert);
	
	//game manager
	//auto gameManagerObj = CreateGameObject();
	//auto gameManager = gameManagerObj->AddComponent<GameManagerBehaviour>();

	//auto resumeBtn = FindTagInChildren(pauseMenuObject, "ResumeBtn");
	//resumeBtn->GetComponent<ButtonComponent>()->SetOnClickFunction(new PauseGameCommand{ gameManager });

//	auto* enemyManagerObj = new GameObject{};
//	auto* pWWm = new WrongWayManager{2, 7};
//	auto* pSSm = new SlickSamManager{ 2, 7 };
//	auto* pCm = new CoilyManager{ 1, 10 };
//	pWWm->SetPyramid(m_pPyramid);
//	pSSm->SetPyramid(m_pPyramid);
//	pCm->SetPyramid(m_pPyramid);
//	enemyManagerObj->AddComponent(pWWm);
//	enemyManagerObj->AddComponent(pSSm);
//	enemyManagerObj->AddComponent(pCm);
//	m_pEnemyManagers.push_back(pWWm);
//	m_pEnemyManagers.push_back(pSSm);
//	m_pEnemyManagers.push_back(pCm);
//	
//	AddObject(enemyManagerObj);
	
//	//Replay
//	pBtnObject = m_pGameOverMenu->FindTagInChildren("ReplayBtn");
//	pBtn = pBtnObject->GetComponent<ButtonComponent>();
//	pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
//	pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
//	pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
//	pBtn->SetOnClickFunction(new ReloadSceneCommand{ this });
//	//Back to main
//	pBtnObject = m_pGameOverMenu->FindTagInChildren("BackToMainBtn");
//	pBtn = pBtnObject->GetComponent<ButtonComponent>();
//	pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
//	pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
//	pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
//	pBtn->SetOnClickFunction(new SwitchScene{ "MainMenuScene" });
//	//quit
//	pBtnObject = m_pGameOverMenu->FindTagInChildren("QuitBtn");
//	pBtn = pBtnObject->GetComponent<ButtonComponent>();
//	pTextComp = pBtnObject->GetComponent<TextRendererComponent>();
//	pBtn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0,}, pTextComp });
//	pBtn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, pTextComp });
//	pBtn->SetOnClickFunction(new QuitGameCommand{  });
//
//	m_pGameOverMenu->SetActive(false);
//
//	auto const pGameManager = new GameManager{ pointsP1->GetComponent<TextRendererComponent>(),
//nullptr, livesP1->GetComponent<TextRendererComponent>(), nullptr, pCm, pWWm, pSSm, m_pGameOverMenu };
//
//	ObserverManager::GetInstance().AddObserver(pGameManager);
//	qbertObj->AddObserver(pGameManager);
//	pWWm->SetGameManager(pGameManager);
//	pSSm->SetGameManager(pGameManager);
//	pCm->SetGameManager(pGameManager);
//
//	auto* camObj = new GameObject{};
//	auto* camComp = new CameraComponent{};
//	camObj->AddComponent(camComp);
//	camObj->GetECSTransform()->Translate(450, 300);
//	SetCameraActive(camComp);
//	AddObject(camObj);

	Instantiate("FPSCounter");
	GetCameraObject()->GetTransform()->Translate(450, 300);
}

void SoloScene::ResetScene(Level newLevel)
{
	m_Level = newLevel;
	//m_pPyramid->Reset();

	for (EnemyManager* pManager : m_pEnemyManagers)
	{
		pManager->Reset();
	}

	//m_pQbert->Reset(false, m_pPyramid->GetTop());
	SetIsPaused(false);
}

void SoloScene::DeclareInput() 
{
	//InputManager::GetInstance().AddInputAction(0, new InputAction(SDLK_z, KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upRight, m_pQbert)));
	//InputManager::GetInstance().AddInputAction(1, new InputAction{ SDLK_d , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downRight, m_pQbert) });
	//InputManager::GetInstance().AddInputAction(2, new InputAction{ SDLK_s , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downLeft, m_pQbert) });
	//InputManager::GetInstance().AddInputAction(3, new InputAction{ SDLK_q , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upLeft, m_pQbert) });

	//InputManager::GetInstance().AddInputAction(4,
	//	new InputAction(ControllerButton::ButtonUp, KeyActionState::pressed,
	//		new MoveCommand(ConnectionDirection::upRight, m_pQbert), PlayerNbr::One));
	//InputManager::GetInstance().AddInputAction(5,
	//	new InputAction{ ControllerButton::ButtonRight , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downRight, m_pQbert), PlayerNbr::One });
	//InputManager::GetInstance().AddInputAction(6,
	//	new InputAction{ ControllerButton::ButtonDown , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::downLeft, m_pQbert), PlayerNbr::One });
	//InputManager::GetInstance().AddInputAction(7,
	//	new InputAction{ ControllerButton::ButtonLeft , KeyActionState::pressed,
	//	new MoveCommand(ConnectionDirection::upLeft, m_pQbert), PlayerNbr::One });
	//
	//InputManager::GetInstance().AddInputAction(100, new InputAction{ SDLK_ESCAPE, KeyActionState::pressed,
	//	new PauseGameCommand(this, m_pPauseMenu) });

	//InputManager::GetInstance().AddInputAction(101, new InputAction{ ControllerButton::Start, KeyActionState::pressed,
	//	new PauseGameCommand(this, m_pPauseMenu), PlayerNbr::One });
}