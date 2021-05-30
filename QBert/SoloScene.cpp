#include "PCH.h"
#include "SoloScene.h"

#include "Pyramid.h"
#include "QBert.h"
#include "MoveCommand.h"
#include "EnemyManager.h"
#include "PauseGameCommand.h"
#include "GameManager.h"

#include "ResourceManager.h"
#include "InputManager.h"
#include "RendererComponent.h"
#include "ObserverManager.h"
#include "PrefabsManager.h"
#include "TextRendererComponent.h"
#include "BoxCollider.h"
#include "CharacterLives.h"

SoloScene::SoloScene()
	:QBertScene("SoloScene")
{
}

void SoloScene::Initialize()
{
	auto const& pPrefabManager = PrefabsManager::GetInstance();
	
	AddObject(pPrefabManager.Instantiate("FPSCounter"));

	auto const livesP1 = pPrefabManager.Instantiate("LivesUI");
	AddObject(livesP1);

	auto const pointsP1 = pPrefabManager.Instantiate("PointsUI");
	AddObject(pointsP1);


	auto qbertObj = pPrefabManager.Instantiate("QBert");
	m_pQbert = qbertObj->GetComponent<QBert>();
	m_pQbert->SetPlayerNbr(1);
	AddObject(qbertObj);

	livesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + 
		std::to_string(qbertObj->GetComponent<CharacterLives>()->GetNbrLives()));

	auto pyramid = pPrefabManager.Instantiate("Pyramid");
	m_pPyramid = pyramid->GetComponent<Pyramid>();
	ObserverManager::GetInstance().AddObserver(new QubeObserver{ m_pPyramid});
	AddObject(pyramid);
	m_pQbert->SetCurrentQube(m_pPyramid->GetTop());
	
	auto enemyManagerObj = new GameObject{};
	m_pManager = new EnemyManager{};
	m_pManager->SetPyramid(m_pPyramid);
	enemyManagerObj->AddComponent(m_pManager);	
	
	AddObject(enemyManagerObj);

	auto pGameManager = new GameManager{ pointsP1->GetComponent<TextRendererComponent>(),
nullptr, livesP1->GetComponent<TextRendererComponent>(), nullptr, m_pPyramid, m_pManager };

	//auto playerObserver = new PlayerObserver{
	//	pointsP1->GetComponent<TextRendererComponent>(), livesP1->GetComponent<TextRendererComponent>(), m_pPyramid, m_pManager };

	ObserverManager::GetInstance().AddObserver(pGameManager);
	qbertObj->AddObserver(pGameManager);

	InputManager::GetInstance().AddInputAction(0, new InputAction(SDLK_w, empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upRight, m_pQbert)));
	InputManager::GetInstance().AddInputAction(1, new InputAction{ SDLK_d , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pQbert) });
	InputManager::GetInstance().AddInputAction(2, new InputAction{ SDLK_s , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pQbert) });
	InputManager::GetInstance().AddInputAction(3, new InputAction{ SDLK_a , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pQbert) });

	InputManager::GetInstance().AddInputAction(4,
		new InputAction(ControllerButton::ButtonUp, empire::KeyActionState::pressed,
			new MoveCommand(ConnectionDirection::upRight, m_pQbert)));
	InputManager::GetInstance().AddInputAction(5,
		new InputAction{ ControllerButton::ButtonRight , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pQbert) });
	InputManager::GetInstance().AddInputAction(6,
		new InputAction{ ControllerButton::ButtonDown , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pQbert) });
	InputManager::GetInstance().AddInputAction(7,
		new InputAction{ ControllerButton::ButtonLeft , empire::KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::upLeft, m_pQbert) });
	
}

void SoloScene::ResetScene(Level newLevel)
{
	m_Level = newLevel;
	m_pPyramid->Reset();
	m_pManager->Reset();
	m_pQbert->Reset(false, m_pPyramid->GetTop());
}

void SoloScene::ResetGame()
{
	m_Level = Level::Level1;
	m_pPyramid->Reset();
	m_pManager->Reset();
	m_pQbert->Reset(true, m_pPyramid->GetTop());
}