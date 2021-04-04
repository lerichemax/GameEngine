#include "PCH.h"
#include "QBertScene.h"

#include "ResourceManager.h"
#include "QubePrefab.h"
#include "GameController.h"
#include "FPSCounter.h"
#include "InputManager.h"

#include "RendererComponent.h"
#include "QBert.h"
#include "MoveCommand.h"
using namespace empire;

QBertScene::QBertScene() : Scene("QBertScene")
{
}

void QBertScene::Initialize()
{
	//auto qubeMiddle = ResourceManager::GetInstance().LoadTexture("Resources/Qube_Middle");
	//auto qubeTop = ResourceManager::GetInstance().LoadTexture("Resources/qubeTop");
	Add(new FPSCounter{});
	
	auto gameController = new GameObject{};
	gameController->GetTransform()->Translate(250.f, 400.f);
	auto gameControllerComp = new GameController{ 7 };
	gameController->AddComponent(gameControllerComp);
	
	Add(gameController);

	auto qbert = new GameObject();
	qbert->AddComponent(new RendererComponent("QBert.png"));
	auto qBertComp = new QBert(gameController->GetComponent<GameController>()->GetTop());
	qbert->AddComponent(qBertComp);
	qbert->GetTransform()->Scale(1.75f);
	Add(qbert);

	InputManager::GetInstance().SetUseKeyboard(true);
	InputManager::GetInstance().AddCommand(SDLK_w, new MoveCommand(ConnectionDirection::upRight, qBertComp, gameControllerComp));
	InputManager::GetInstance().AddCommand(SDLK_d, new MoveCommand(ConnectionDirection::downRight, qBertComp, gameControllerComp));
	InputManager::GetInstance().AddCommand(SDLK_s, new MoveCommand(ConnectionDirection::downLeft, qBertComp, gameControllerComp));
	InputManager::GetInstance().AddCommand(SDLK_a, new MoveCommand(ConnectionDirection::upLeft, qBertComp, gameControllerComp));
}