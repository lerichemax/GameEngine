#include "PCH.h"
#include "VersusScene.h"

#include "UIManagerSystem.h"
#include "DiskSystem.h"
#include "PyramidSystem.h"
#include "QubeSystem.h"
#include "GameManagerSystem.h"
#include "QBertSystem.h"
#include "CharacterControllerSystem.h"
#include "CharacterMovementSystem.h"
#include "JumperSystem.h"
#include "LivesSystem.h"
#include "AiControllerSystem.h"
#include "CoilySystem.h"

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
	AddSystem<PyramidSystem>();
	AddSystem<DiskSystem>();
	AddSystem<QubeSystem>();

	//Qbert
	auto pQbertObj = Instantiate("QBert");
	pQbertObj->GetComponent<CharacterLives>()->Init(4);
	AddSystem<QBertSystem>();
	AddSystem<CharacterControllerSystem>();
	AddSystem<CharacterMovementSystem>();
	AddSystem<JumperSystem>();
	AddSystem<LivesSystem>();

	//game manager
	auto* const pGameManager = AddSystem<GameManagerSystem>();
	pGameManager->SetGameMode(GameMode::Versus);

	pPauseMenuObject->FindChildrenWithTag("ResumeBtn")->GetComponent<ButtonComponent>()->SetOnClickFunction( PauseGameCommand{ pGameManager });

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
	pPointsP2->GetTransform()->SetLocation(20.f, 100.f);

	AddSystem<AiControllerSystem>();
	AddSystem<CoilySystem>();
}
