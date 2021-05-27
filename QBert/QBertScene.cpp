#include "PCH.h"
#include "QBertScene.h"

#include "Coily.h"
#include "SlickSam.h"
#include "ColoredDisk.h"
#include "Pyramid.h"
#include "QBert.h"
#include "MoveCommand.h"
#include "PlayerObserver.h"
#include "WrongWay.h"
#include "Jumper.h"
#include "WrongWayJumper.h"
#include "EnemyManager.h"

#include "ResourceManager.h"
#include "InputManager.h"
#include "RendererComponent.h"
#include "ObserverManager.h"
#include "Subject.h"
#include "PrefabsManager.h"
#include "TextRendererComponent.h"
#include "BoxCollider.h"

using namespace empire;

QBertScene::QBertScene(Level startLevel)
	: Scene("QBertScene"),
	m_Level(startLevel),
	m_pQbert(nullptr),
	m_pPyramid(nullptr),
	m_pManager(nullptr)
{
}

void QBertScene::Initialize()
{
	AddObject(PrefabsManager::GetInstance().Instantiate("FPSCounter"));

	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 15);
	auto const& pPrefabManager = PrefabsManager::GetInstance();
	
	auto const livesP1 = pPrefabManager.Instantiate("LivesUI");
	AddObject(livesP1);

	auto const pointsP1 = pPrefabManager.Instantiate("PointsUI");
	AddObject(pointsP1);

	auto qbertObj = pPrefabManager.Instantiate("QBert");
	m_pQbert = qbertObj->GetComponent<QBert>();
	m_pQbert->SetPlayerNbr(1);
	AddObject(qbertObj);
	
	livesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + std::to_string(m_pQbert->GetLives()));

	auto pyramid = pPrefabManager.Instantiate("Pyramid");
	m_pPyramid = pyramid->GetComponent<Pyramid>();
	ObserverManager::GetInstance().AddObserver(new QubeObserver{ m_pPyramid, m_pQbert });
	AddObject(pyramid);
	
	m_pQbert->SetCurrentQube(m_pPyramid->GetTop());

	auto enemyManagerObj = new GameObject{};
	m_pManager = new EnemyManager{};
	m_pManager->SetPyramid(m_pPyramid);
	enemyManagerObj->AddComponent(m_pManager);
	AddObject(enemyManagerObj);

	ObserverManager::GetInstance().AddObserver(new EnemyObserver{ m_pManager });
	
	auto playerObserver = new PlayerObserver{
		pointsP1->GetComponent<TextRendererComponent>(), livesP1->GetComponent<TextRendererComponent>(), m_pPyramid, m_pManager };
	ObserverManager::GetInstance().AddObserver(playerObserver);
	m_pQbert->GetSubject()->AddObserver(playerObserver);
	
	InputManager::GetInstance().AddCommand(SDLK_w, new MoveCommand(ConnectionDirection::upRight, m_pQbert, m_pPyramid));
	InputManager::GetInstance().AddCommand(SDLK_d, new MoveCommand(ConnectionDirection::downRight, m_pQbert, m_pPyramid));
	InputManager::GetInstance().AddCommand(SDLK_s, new MoveCommand(ConnectionDirection::downLeft, m_pQbert, m_pPyramid));
	InputManager::GetInstance().AddCommand(SDLK_a, new MoveCommand(ConnectionDirection::upLeft, m_pQbert, m_pPyramid));
}

void QBertScene::ResetScene(Level newLevel)
{
	m_Level = newLevel;
	m_pPyramid->Reset();
	m_pManager->Reset();
	m_pQbert->Reset(false, m_pPyramid->GetTop());
}

void QBertScene::ResetGame()
{
	m_Level = Level::Level1;
	m_pPyramid->Reset();
	m_pManager->Reset();
	m_pQbert->Reset(true, m_pPyramid->GetTop());
}