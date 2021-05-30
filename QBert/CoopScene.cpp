#include "PCH.h"
#include "CoopScene.h"


#include "CharacterLives.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "ObserverManager.h"
#include "PrefabsManager.h"
#include "QBert.h"
#include "QubeObserver.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"
#include "EnemyManager.h"
#include "GameManager.h"
#include "PauseGameCommand.h"

using namespace empire;
CoopScene::CoopScene() : QBertScene("CoopScene")
{
	
}

void CoopScene::Initialize()
{
	AddObject(empire::PrefabsManager::GetInstance().Instantiate("FPSCounter"));

	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 15);
	
	auto const livesP1 = new GameObject;
	livesP1->AddComponent(new TextRendererComponent{ "P1 Lives:", font });
	livesP1->GetTransform()->Translate(20.f, 40.f);
	livesP1->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	AddObject(livesP1);

	auto const pointsP1 = new GameObject;
	pointsP1->AddComponent(new TextRendererComponent{ "P1 Points: 0 " , font });
	pointsP1->GetTransform()->Translate(20.f, 60.f);
	pointsP1->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	AddObject(pointsP1);

	auto const livesP2 = new GameObject;
	livesP2->AddComponent(new TextRendererComponent{ "P2 Lives: ", font });
	livesP2->GetTransform()->Translate(20.f, 80.f);
	livesP2->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 255);
	AddObject(livesP2);

	auto const pointsP2 = new GameObject{};
	pointsP2->AddComponent(new TextRendererComponent{ "P2 Points: 0 " , font });
	pointsP2->GetTransform()->Translate(20.f, 100.f);
	pointsP2->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 255);
	AddObject(pointsP2);

	auto qbert1 = PrefabsManager::GetInstance().Instantiate("QBert");
	m_pQbert = qbert1->GetComponent<QBert>();
	m_pQbert->SetPlayerNbr(1);
	AddObject(qbert1);
	
	auto qbert2 = PrefabsManager::GetInstance().Instantiate("QBert");
	m_pQbertP2 = qbert2->GetComponent<QBert>();
	m_pQbertP2->SetPlayerNbr(2);
	AddObject(qbert2);

	livesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + 
		std::to_string(qbert1->GetComponent<CharacterLives>()->GetNbrLives()));
	livesP2->GetComponent<TextRendererComponent>()->SetText("P2 Lives: " + 
		std::to_string(qbert2->GetComponent<CharacterLives>()->GetNbrLives()));
	
	auto pyramid = PrefabsManager::GetInstance().Instantiate("Pyramid");
	m_pPyramid = pyramid->GetComponent<Pyramid>();
	ObserverManager::GetInstance().AddObserver(new QubeObserver{ m_pPyramid});
	AddObject(pyramid);

	auto enemyManagerObj = new GameObject{};
	m_pManager = new EnemyManager{};
	m_pManager->SetPyramid(m_pPyramid);
	enemyManagerObj->AddComponent(m_pManager);
	AddObject(enemyManagerObj);

	m_pQbert->SetCurrentQube(m_pPyramid->GetEscheresqueLeftTop());
	m_pQbertP2->SetCurrentQube(m_pPyramid->GetEscheresqueRightTop());


	//auto p1Observer = new PlayerObserver{ pointsP1->GetComponent<TextRendererComponent>(),
	//livesP1->GetComponent<TextRendererComponent>(), m_pPyramid, m_pManager };

	//auto p2Observer = new PlayerObserver{ pointsP2->GetComponent<TextRendererComponent>(),
	//livesP2->GetComponent<TextRendererComponent>(), m_pPyramid, m_pManager };

	auto pGameManager = new GameManager{ pointsP1->GetComponent<TextRendererComponent>(),
	pointsP2->GetComponent<TextRendererComponent>(), livesP1->GetComponent<TextRendererComponent>(),
		livesP2->GetComponent<TextRendererComponent>(), m_pPyramid, m_pManager,2 };

	ObserverManager::GetInstance().AddObserver(pGameManager);
	
	qbert1->AddObserver(pGameManager);
	qbert2->AddObserver(pGameManager);
	
	//Player1 input
	//InputManager::GetInstance().AddCommand(SDLK_w, new MoveCommand(ConnectionDirection::upRight, m_pQbert));
	//InputManager::GetInstance().AddCommand(SDLK_d, new MoveCommand(ConnectionDirection::downRight, m_pQbert));
	//InputManager::GetInstance().AddCommand(SDLK_s, new MoveCommand(ConnectionDirection::downLeft, m_pQbert));
	//InputManager::GetInstance().AddCommand(SDLK_a, new MoveCommand(ConnectionDirection::upLeft, m_pQbert));

	////Player2 input
	//InputManager::GetInstance().AddCommand(SDLK_UP, new MoveCommand(ConnectionDirection::upRight, m_pQbertP2));
	//InputManager::GetInstance().AddCommand(SDLK_RIGHT, new MoveCommand(ConnectionDirection::downRight, m_pQbertP2));
	//InputManager::GetInstance().AddCommand(SDLK_DOWN, new MoveCommand(ConnectionDirection::downLeft, m_pQbertP2));
	//InputManager::GetInstance().AddCommand(SDLK_LEFT, new MoveCommand(ConnectionDirection::upLeft, m_pQbertP2));

	//InputManager::GetInstance().AddCommand(SDLK_ESCAPE, new PauseGameCommand(KeyActionState::pressed));
}

void CoopScene::ResetScene(Level newLevel)
{
	m_Level = newLevel;
	m_pPyramid->Reset();
	m_pManager->Reset();
	m_pQbert->Reset(false, m_pPyramid->GetEscheresqueLeftTop());
	m_pQbert->GetGameObject()->SetActive(true);
	m_pQbertP2->Reset(false, m_pPyramid->GetEscheresqueRightTop());
	m_pQbertP2->GetGameObject()->SetActive(true);
}

void CoopScene::ResetGame()
{
	m_Level = Level::Level1;
	m_pPyramid->Reset();
	m_pManager->Reset();
	m_pQbert->GetGameObject()->SetActive(true);
	m_pQbertP2->GetGameObject()->SetActive(true);
	m_pQbert->Reset(true, m_pPyramid->GetEscheresqueLeftTop());
	m_pQbertP2->Reset(true, m_pPyramid->GetEscheresqueRightTop());
}