#include "PCH.h"
#include "SoloScene.h"

#include "PyramidSystem.h"
#include "QBertSystem.h"
#include "QubeSystem.h"
#include "DiskSystem.h"
#include "EnemySpawnerSystem.h"
#include "UIManagerSystem.h"
#include "GameManagerSystem.h"
#include "CharacterControllerSystem.h"
#include "CharacterMovementSystem.h"
#include "JumperSystem.h"
#include "QubeSystem.h"
#include "PyramidSystem.h"
#include "AiControllerSystem.h"
#include "CoilySystem.h"
#include "LivesSystem.h"
#include "FPSCounterSystem.h"

#include "UiManagerComponent.h"
#include "EnemySpawnerComponent.h"
#include "AiControllerComponent.h"
#include "TextRendererComponent.h"
#include "ButtonComponent.h"
#include "CharacterLives.h"
#include "MovementComponent.h"

#include "PauseGameCommand.h"
#include "QuitGameCommand.h"
#include "ReloadSceneCommand.h"
#include "SwitchScene.h"
#include "SwitchTextColor.h"

#include "ResourceManager.h"
#include "InputManager.h"
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

	auto pLivesP1 = Instantiate("LivesUI");
	auto pPointsP1 = Instantiate("PointsUI");

	auto pUiManagerObj = CreateGameObject();
	auto pUiComp = pUiManagerObj->AddComponent<UiManagerComponent>();
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
	AddSystem<PyramidSystem>();
	AddSystem<DiskSystem>();
	AddSystem<QubeSystem>();

	//Qbert
	auto pQbertObj = Instantiate("QBert");
	AddSystem<QBertSystem>();
	AddSystem<CharacterControllerSystem>();
	AddSystem<CharacterMovementSystem>();
	AddSystem<JumperSystem>();
	AddSystem<LivesSystem>();

	pLivesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + std::to_string(pQbertObj->GetComponent<CharacterLives>()->NbrLives));

	//game manager
	auto* const pGameManager = AddSystem<GameManagerSystem>();
	pGameManager->SetGameMode(GameMode::Normal);

	pPauseMenuObject->FindChildrenWithTag("ResumeBtn")->GetComponent<ButtonComponent>()->SetOnClickFunction(new PauseGameCommand{ pGameManager });

	//ENEMIES SPAWNERS
	//AddSystem<EnemySpawnerSystem>();
	//AddSystem<AiControllerSystem>();
	//AddSystem<CoilySystem>();
	
	// Slick Sam
	auto pSlickSamSpawnerObj = CreateGameObject();
	auto* pSpawnerComp = pSlickSamSpawnerObj->AddComponent<EnemySpawnerComponent>();
	pSpawnerComp->MaxEnemies = 2;
	pSpawnerComp->SpawnInterval = 7;
	pSpawnerComp->Type = EnemyType::SlickSam;
	//pSpawnerComp->SpawnedEnemies.push_back(Instantiate("Slick")->GetEntity());
	//pSpawnerComp->SpawnedEnemies.push_back(Instantiate("Sam")->GetEntity());

	//	Coily
	auto pCoilySpawnerObj = CreateGameObject();
	pSpawnerComp = pCoilySpawnerObj->AddComponent<EnemySpawnerComponent>();
	pSpawnerComp->MaxEnemies = 1;
	pSpawnerComp->SpawnInterval = 10;
	pSpawnerComp->Type = EnemyType::Coily;
	//pSpawnerComp->SpawnedEnemies.push_back(Instantiate("Coily")->GetEntity());

	//	Ugg
	auto pUggObject = CreateGameObject();
	pSpawnerComp = pUggObject->AddComponent<EnemySpawnerComponent>();
	pSpawnerComp->MaxEnemies = 2;
	pSpawnerComp->SpawnInterval = 6;
	pSpawnerComp->Type = EnemyType::WrongWay; //ugg and wrong way use the same type
	//pSpawnerComp->SpawnedEnemies.push_back(Instantiate("Ugg")->GetEntity());
	//pSpawnerComp->SpawnedEnemies.push_back(Instantiate("WrongWay")->GetEntity());
	
	Instantiate("FPSCounter");
	AddSystem<FPSCounterSystem>();
}