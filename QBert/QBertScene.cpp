#include "PCH.h"
#include "QBertScene.h"

#include "ResourceManager.h"
#include "FPSCounter.h"
#include "InputManager.h"
#include "UIObject.h"
#include "RendererComponent.h"
#include "ObserverManager.h"
#include "Subject.h"

#include "Pyramid.h"
#include "QBert.h"
#include "MoveCommand.h"
#include "PlayerObserver.h"
#include "CoilyPrefab.h"

using namespace empire;

QBertScene::QBertScene(Level startLevel)
	: Scene("QBertScene"),
	m_Level(startLevel)
{
}

void QBertScene::Initialize()
{
	Add(new FPSCounter{});
	
	auto gameController = new GameObject{};
	gameController->GetTransform()->Translate(250.f, 400.f);
	m_pPyramid = new Pyramid{ 7 };
	gameController->AddComponent(m_pPyramid);

	Add(gameController);
	
	auto const livesP1 = new UIObject{};
	livesP1->GetComponent<TransformComponent>()->Translate(20.f, 40.f, 0.f);
	livesP1->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	Add(livesP1);

	auto const pointsP1 = new UIObject{};
	pointsP1->GetComponent<TransformComponent>()->Translate(20.f, 60.f, 0.f);
	pointsP1->GetComponent<TextRendererComponent>()->SetText("P1 Points: 0 ");
	pointsP1->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	Add(pointsP1);
	
	auto qbert = new GameObject();
	qbert->AddComponent(new RendererComponent("QBert.png"));
	
	m_pQbert = new QBert();
	qbert->AddComponent(m_pQbert);
	qbert->GetTransform()->Scale(1.75f);
	
	livesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + std::to_string(m_pQbert->GetLives()));
	
	Add(qbert);
	
	m_pPyramid->InitializeQubes(m_pQbert);
	m_pQbert->SetCurrentQube(m_pPyramid->GetTop());
	
	auto playerObserver = new PlayerObserver{ pointsP1, livesP1, m_pPyramid };
	ObserverManager::GetInstance().AddObserver(playerObserver);
	m_pQbert->GetSubject()->AddObserver(playerObserver);

	/*auto pEnemy = new CoilyPrefab(m_pPyramid->GetQube(2), m_pPyramid);
	Add(pEnemy);*/
	
	
	InputManager::GetInstance().SetUseKeyboard(true);
	InputManager::GetInstance().AddCommand(SDLK_w, new MoveCommand(ConnectionDirection::upRight, m_pQbert, m_pPyramid));
	InputManager::GetInstance().AddCommand(SDLK_d, new MoveCommand(ConnectionDirection::downRight, m_pQbert, m_pPyramid));
	InputManager::GetInstance().AddCommand(SDLK_s, new MoveCommand(ConnectionDirection::downLeft, m_pQbert, m_pPyramid));
	InputManager::GetInstance().AddCommand(SDLK_a, new MoveCommand(ConnectionDirection::upLeft, m_pQbert, m_pPyramid));
}

void QBertScene::ResetScene(Level newLevel)
{
	m_Level = newLevel;
	m_pQbert->Reset(false, m_pPyramid->GetTop());
	m_pPyramid->Reset();
}

void QBertScene::ResetGame()
{
	m_Level = Level::Level1;
	m_pQbert->Reset(true, m_pPyramid->GetTop());
	m_pPyramid->Reset();
}