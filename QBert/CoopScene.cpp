#include "PCH.h"
#include "CoopScene.h"

#include "UIManagerSystem.h"
#include "DiskSystem.h"
#include "PyramidSystem.h"
#include "EnemySpawnerSystem.h"
#include "GameManagerSystem.h"

#include "UiManagerComponent.h"
#include "TextRendererComponent.h"
#include "CharacterLives.h"
#include "MovementComponent.h"
#include "CharacterControllerComponent.h"
#include "EnemySpawnerComponent.h"
#include "AiControllerComponent.h"
#include "QbertComponent.h"
#include "ButtonComponent.h"

#include "PauseGameCommand.h"


CoopScene::CoopScene()
	: Scene("CoopScene")
{
}

void CoopScene::Initialize()
{
	auto pLivesP1 = Instantiate("LivesUI");
	auto pPointsP1 = Instantiate("PointsUI");

	auto pLivesP2 = Instantiate("LivesUI");
	auto pPointsP2 = Instantiate("PointsUI");

	auto pUiManagerObj = CreateGameObject();
	auto pUiComp = pUiManagerObj->AddComponent<UiManagerComponent>();
	pUiComp->LivesCounterTextEntity = pLivesP1->GetEntity();
	pUiComp->PointsCounterTextEntity = pPointsP1->GetEntity();
	pUiComp->LivesCounterTextEntityP2 = pLivesP2->GetEntity();
	pUiComp->PointsCounterTextEntityP2 = pPointsP2->GetEntity();

	AddSystem<UIManagerSystem>();

	//Qberts
	auto pQbertObj = Instantiate("QBert");
	auto pQbertObj2 = Instantiate("QBert");

	auto pMovementComp = pQbertObj2->GetComponent<MovementComponent>();
	pMovementComp->SetTextureIdleNames("Textures/QBert/QBert2_DownRight_Qube.png", "Textures/QBert/QBert2_DownLeft_Qube.png",
		"Textures/QBert/QBert2_UpRight_Qube.png", "Textures/QBert/QBert2_UpLeft_Qube.png");
	pMovementComp->SetTextureJumpNames("Textures/QBert/QBert2_DownRight_Jump.png", "Textures/QBert/QBert2_DownLeft_Jump.png",
		"Textures/QBert/QBert2_UpRight_Jump.png", "Textures/QBert/QBert2_UpLeft_Jump.png");

	auto pCharacterController = pQbertObj2->AddComponent<CharacterControllerComponent>();
	pCharacterController->Right = SDL_SCANCODE_RIGHT;
	pCharacterController->Left = SDL_SCANCODE_LEFT;
	pCharacterController->Up = SDL_SCANCODE_UP;
	pCharacterController->Down = SDL_SCANCODE_DOWN;

	pQbertObj2->GetComponent<QbertComponent>()->PlayerNumber = 2;

	pLivesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + std::to_string(pQbertObj->GetComponent<CharacterLives>()->NbrLives));

	auto pTextLivesP2 = pLivesP2->GetComponent<TextRendererComponent>();
	pTextLivesP2->SetText("P2 Lives: " + std::to_string(pQbertObj2->GetComponent<CharacterLives>()->NbrLives));
	pTextLivesP2->SetTextColor(255, 0, 255);
	pLivesP2->GetTransform()->Translate(20.f, 80.f);
	
	auto pTextPointsP2 = pPointsP2->GetComponent<TextRendererComponent>();
	pTextPointsP2->SetTextColor(255, 0, 255);
	pPointsP2->GetTransform()->Translate(20.f, 100.f);

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

	//ENEMIES SPAWNERS
	AddSystem<EnemySpawnerSystem>();

	// Slick Sam
	auto pSlickSamSpawnerObj = CreateGameObject();
	auto* pSpawnerComp = pSlickSamSpawnerObj->AddComponent<EnemySpawnerComponent>();
	pSpawnerComp->MaxEnemies = 2;
	pSpawnerComp->SpawnInterval = 7;
	pSpawnerComp->Type = EnemyType::SlickSam;
	pSpawnerComp->SpawnedEnemies.push_back(Instantiate("Slick")->GetEntity());
	pSpawnerComp->SpawnedEnemies.push_back(Instantiate("Sam")->GetEntity());

	//	Coily
	auto pCoilySpawnerObj = CreateGameObject();
	pSpawnerComp = pCoilySpawnerObj->AddComponent<EnemySpawnerComponent>();
	pSpawnerComp->MaxEnemies = 1;
	pSpawnerComp->SpawnInterval = 10;
	pSpawnerComp->Type = EnemyType::Coily;
	pSpawnerComp->SpawnedEnemies.push_back(Instantiate("Coily")->GetEntity());

	//	Ugg
	auto pUggObject = CreateGameObject();
	pSpawnerComp = pUggObject->AddComponent<EnemySpawnerComponent>();
	pSpawnerComp->MaxEnemies = 2;
	pSpawnerComp->SpawnInterval = 6;
	pSpawnerComp->Type = EnemyType::WrongWay; //ugg and wrong way use the same type
	pSpawnerComp->SpawnedEnemies.push_back(Instantiate("Ugg")->GetEntity());
	pSpawnerComp->SpawnedEnemies.push_back(Instantiate("WrongWay")->GetEntity());

	auto* const pGameManager = AddSystem<GameManagerSystem>();
	pGameManager->SetGameMode(GameMode::Coop);

	pPauseMenuObject->FindChildrenWithTag("ResumeBtn")->GetComponent<ButtonComponent>()->SetOnClickFunction(new PauseGameCommand{ pGameManager });

	Instantiate("FPSCounter");
	GetCameraObject()->GetTransform()->Translate(450, 300);
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
//	m_pQbert->GetEntity()->SetActive(true);
//	m_pQbertP2->GetEntity()->SetActive(true);
//	//m_pQbert->Reset(true, m_pPyramid->GetEscheresqueLeftTop());
//	//m_pQbertP2->Reset(true, m_pPyramid->GetEscheresqueRightTop());
//	SetIsPaused(false);
//	m_pPauseMenu->SetActive(false);
//	m_pGameOverMenu->SetActive(false);
//}