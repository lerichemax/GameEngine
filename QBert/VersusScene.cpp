#include "PCH.h"
#include "VersusScene.h"


#include "CharacterLives.h"
#include "CharacterPoint.h"
#include "Coily.h"
#include "CoilyCharacterController.h"
#include "Command.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "ObserverManager.h"
#include "PauseGameCommand.h"
#include "QBert.h"
#include "QubeObserver.h"
#include "VersusGameManager.h"
#include "TextRendererComponent.h"

#include "PrefabsManager.h"
#include "ResourceManager.h"


using namespace empire;

VersusScene::VersusScene() : QBertScene("VersusScene")
{
}

void VersusScene::Initialize()
{
	auto const& pPrefabManager = PrefabsManager::GetInstance();
	
	AddObject(pPrefabManager.Instantiate("FPSCounter"));

	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 15);
	
	auto const pointsP1 = new GameObject{};
	pointsP1->GetTransform()->Translate(20.f, 60.f);
	pointsP1->AddComponent(new TextRendererComponent{ "P1: 0 ", font });
	pointsP1->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	m_pTextP1 = pointsP1->GetComponent<TextRendererComponent>();
	AddObject(pointsP1);
	
	auto const pointsP2 = new GameObject{};
	pointsP2->GetTransform()->Translate(20.f, 80.f);
	pointsP2->AddComponent(new TextRendererComponent{ "P2: 0 ", font });
	pointsP2->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 255);
	m_pTextP2 = pointsP2->GetComponent<TextRendererComponent>();
	AddObject(pointsP2);

	auto const fontBig = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 24);
	
	auto const roundTxt = new GameObject{};
	roundTxt->GetTransform()->Translate(400.f, 60.f);
	roundTxt->AddComponent(new TextRendererComponent{ "Round 1", fontBig });
	roundTxt->GetComponent<TextRendererComponent>()->SetTextColor(255, 255, 255);
	m_pRoundText = roundTxt->GetComponent<TextRendererComponent>();
	AddObject(roundTxt);
	
	auto qbertObj = pPrefabManager.Instantiate("QBert");
	m_pQbert = qbertObj->GetComponent<QBert>();
	m_pQbert->SetPlayerNbr(1);
	AddObject(qbertObj);
	
	auto pyramid = pPrefabManager.Instantiate("Pyramid");
	m_pPyramid = pyramid->GetComponent<Pyramid>();
	ObserverManager::GetInstance().AddObserver(new QubeObserver{ m_pPyramid });
	AddObject(pyramid);
	m_pQbert->SetCurrentQube(m_pPyramid->GetTop());

	auto coilyObj = pPrefabManager.Instantiate("Coily");
	m_pCoilyPlayer = coilyObj->GetComponent<Coily>();
	m_pCoilyPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
	coilyObj->AddComponent(new CharacterPoint{});
	coilyObj->AddComponent(new CharacterLives{ 3 });
	coilyObj->GetComponent<CoilyCharacterController>()->SetPyramid(m_pPyramid);
	AddObject(coilyObj);

	auto pGameManager = new VersusGameManager{ m_pRoundText, m_pTextP1, m_pTextP2,
		qbertObj->GetComponent<CharacterPoint>(), coilyObj->GetComponent<CharacterPoint>(), m_pPyramid, 3 };
	ObserverManager::GetInstance().AddObserver(pGameManager);
	
	qbertObj->AddObserver(pGameManager);
	coilyObj->AddObserver(pGameManager);
	pyramid->AddObserver(pGameManager);
	
	InputManager::GetInstance().AddCommand(SDLK_w, new MoveCommand(ConnectionDirection::upRight, m_pQbert));
	InputManager::GetInstance().AddCommand(SDLK_d, new MoveCommand(ConnectionDirection::downRight, m_pQbert));
	InputManager::GetInstance().AddCommand(SDLK_s, new MoveCommand(ConnectionDirection::downLeft, m_pQbert));
	InputManager::GetInstance().AddCommand(SDLK_a, new MoveCommand(ConnectionDirection::upLeft, m_pQbert));
	InputManager::GetInstance().AddCommand(SDLK_ESCAPE, new PauseGameCommand(KeyActionState::pressed));
}

void VersusScene::ResetGame()
{
	m_pPyramid->Reset();
	
	m_pQbert->Reset(true, m_pPyramid->GetTop());
	m_pQbert->GetGameObject()->GetComponent<empire::RendererComponent>()->ChangeLayer(empire::Layer::foreground);
	m_pQbert->SetCanMove();
	
	m_pCoilyPlayer->Transform(false);
	m_pCoilyPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
	m_pCoilyPlayer->GetGameObject()->GetTransform()->Translate(m_pCoilyPlayer->GetCurrentQube()->GetCharacterPos());
	m_pCoilyPlayer->GetGameObject()->GetComponent<CoilyCharacterController>()->SetEnable(true);
	m_pCoilyPlayer->GetGameObject()->GetComponent<CharacterPoint>()->Reset();
	
	m_pRoundText->SetText("Round 1");
	m_pTextP1->SetText("P1: 0");
	m_pTextP2->SetText("P2: 0");
}

void VersusScene::ResetScene(Level ) //ignore level, always resets to level 1
{
	m_pPyramid->Reset();
	m_pQbert->Reset(false, m_pPyramid->GetTop());
	
	m_pCoilyPlayer->Transform(false);
	m_pCoilyPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
	m_pCoilyPlayer->GetGameObject()->GetTransform()->Translate(m_pCoilyPlayer->GetCurrentQube()->GetCharacterPos());
	m_pCoilyPlayer->GetGameObject()->GetComponent<CoilyCharacterController>()->SetEnable(true);
}