#include "PCH.h"
#include "SoloScene.h"

#include "PyramidSystem.h"
#include "QBertSystem.h"
#include "QubeSystem.h"
#include "DiskSystem.h"
#include "EnemySpawnerSystem.h"
#include "UIManagerSystem.h"
#include "GameManagerSystem.h"

#include "UiManagerComponent.h"
#include "EnemySpawnerComponent.h"
#include "AiControllerComponent.h"
#include "CameraComponent.h"
#include "TextRendererComponent.h"
#include "ButtonComponent.h"
#include "CharacterLives.h"

#include "PauseGameCommand.h"
#include "QuitGameCommand.h"
#include "ReloadSceneCommand.h"
#include "SwitchScene.h"
#include "SwitchTextColor.h"

#include "ResourceManager.h"
#include "InputManager.h"
#include "ObserverManager.h"
#include "PrefabsManager.h"
#include "JsonReaderWriter.h"
#include "Timer.h"

#include "../3rdParty/imgui/imgui.h"

SoloScene::SoloScene()
	:Scene("SoloScene")
{
}

void SoloScene::Initialize()
{
	auto const pLivesP1 = Instantiate("LivesUI");
	auto const pPointsP1 = Instantiate("PointsUI");

	auto* const pUiManagerObj = CreateGameObject();
	auto* const pUiComp = pUiManagerObj->AddComponent<UiManagerComponent>();
	pUiComp->LivesCounterTextEntity = pLivesP1->GetEntity();
	pUiComp->PointsCounterTextEntity = pPointsP1->GetEntity();

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
	auto* const pQbertObj = Instantiate("QBert");

	pLivesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + std::to_string(m_pRegistry->GetComponent<CharacterLives>(pQbertObj->GetEntity())->NbrLives));
	
	//game manager
	auto* const pGameManager = AddSystem<GameManagerSystem>();

	auto* const pResumeBtn = FindTagInChildren(pPauseMenuObject, "ResumeBtn");
	pResumeBtn->GetComponent<ButtonComponent>()->SetOnClickFunction(new PauseGameCommand{ pGameManager });

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

	//ENEMIES SPAWNERS
	AddSystem<EnemySpawnerSystem>();
	
	// Slick Sam
	auto* const pSlickSamSpawnerObj = CreateGameObject();
	auto* pSpawnerComp = pSlickSamSpawnerObj->AddComponent<EnemySpawnerComponent>();
	pSpawnerComp->MaxEnemies = 2;
	pSpawnerComp->SpawnInterval = 7;
	pSpawnerComp->Type = EnemyType::SlickSam;
	pSpawnerComp->SpawnedEnemies.push_back(Instantiate("Slick")->GetEntity());
	pSpawnerComp->SpawnedEnemies.push_back(Instantiate("Sam")->GetEntity());

	//	Coily
	auto* const pCoilySpawnerObj = CreateGameObject();
	pSpawnerComp = pCoilySpawnerObj->AddComponent<EnemySpawnerComponent>();
	pSpawnerComp->MaxEnemies = 1;
	pSpawnerComp->SpawnInterval = 10;
	pSpawnerComp->Type = EnemyType::Coily;
	pSpawnerComp->SpawnedEnemies.push_back(Instantiate("Coily")->GetEntity());


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

//void SoloScene::DeclareInput() 
//{
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
//}