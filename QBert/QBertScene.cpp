#include "PCH.h"
#include "QBertScene.h"

#include "Coily.h"
#include "SlickSam.h"
#include "ColoredDisk.h"
#include "Pyramid.h"
#include "QBert.h"
#include "MoveCommand.h"
#include "PlayerObserver.h"


#include "ResourceManager.h"
#include "InputManager.h"
#include "RendererComponent.h"
#include "ObserverManager.h"
#include "Subject.h"
#include "PrefabsManager.h"
#include "TextRendererComponent.h"

using namespace empire;

QBertScene::QBertScene(Level startLevel)
	: Scene("QBertScene"),
	m_Level(startLevel)
{
}

void QBertScene::Initialize()
{
	Add(PrefabsManager::GetInstance().Instantiate("FPSCounter"));
	
	auto const livesP1 = PrefabsManager::GetInstance().Instantiate("UIObject", glm::vec3{ 20.f, 40.f, 0.f });
	livesP1->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	Add(livesP1);

	auto const pointsP1 = PrefabsManager::GetInstance().Instantiate("UIObject", glm::vec3{ 20.f, 60.f, 0.f });
	pointsP1->GetComponent<TextRendererComponent>()->SetText("P1 Points: 0 ");
	pointsP1->GetComponent<TextRendererComponent>()->SetTextColor(255, 0, 0);
	Add(pointsP1);
	
	auto qbert = new GameObject();
	qbert->AddComponent(new RendererComponent("QBert.png", Layer::foreground));
	
	m_pQbert = new QBert();
	qbert->AddComponent(m_pQbert);
	qbert->GetTransform()->Scale(1.5f);
	
	livesP1->GetComponent<TextRendererComponent>()->SetText("P1 Lives: " + std::to_string(m_pQbert->GetLives()));

	//Qube prefab
	auto qubePf = new GameObject{};
	auto text = ResourceManager::GetInstance().GetTexture("Qube.png");
	auto interText = ResourceManager::GetInstance().GetTexture("Qube_Intermediate.png");
	auto flippedText = ResourceManager::GetInstance().GetTexture("Qube_Flipped.png");
	qubePf->GetTransform()->Scale(1.75f);
	qubePf->AddComponent(new RendererComponent(text));
	qubePf->AddComponent(new Qube{ text, interText, flippedText });
	PrefabsManager::GetInstance().AddPrefab("Qube", qubePf);
	
	auto pyramid = new GameObject{};
	pyramid->GetTransform()->Translate(250.f, 400.f);
	m_pPyramid = new Pyramid{ 7, m_pQbert };
	pyramid->AddComponent(m_pPyramid);

	Add(pyramid);
	Add(qbert);
	
	m_pQbert->SetCurrentQube(m_pPyramid->GetTop());

	//Coily prefab
	auto coilyPrefab = new GameObject{};
	auto pText = empire::ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Egg_Small.png");
	coilyPrefab->AddComponent(new empire::RendererComponent(pText, empire::Layer::middleground));
	coilyPrefab->AddComponent(new Coily{m_pPyramid});
	coilyPrefab->GetTransform()->Scale(1.5f);
	PrefabsManager::GetInstance().AddPrefab("Coily", coilyPrefab);

	//SlickSam
	auto slickSamPf = new GameObject();
	slickSamPf->AddComponent(new empire::RendererComponent(empire::Layer::middleground));
	slickSamPf->AddComponent(new SlickSam{});
	slickSamPf->GetTransform()->Scale(1.5f);
	PrefabsManager::GetInstance().AddPrefab("SlickSam", slickSamPf);

	//ColoredDisk
	auto diskPf = new GameObject{};
	auto const diskText = empire::ResourceManager::GetInstance().GetTexture("Disk.png");
	diskPf->AddComponent(new ColoredDisk{m_pPyramid->GetTop()});
	diskPf->AddComponent(new empire::RendererComponent{ diskText, empire::Layer::middleground });
	diskPf->GetTransform()->Scale(2);
	PrefabsManager::GetInstance().AddPrefab("Disk", diskPf);
	
	auto playerObserver = new PlayerObserver{
		pointsP1->GetComponent<TextRendererComponent>(), livesP1->GetComponent<TextRendererComponent>(), m_pPyramid };
	ObserverManager::GetInstance().AddObserver(20, playerObserver);
	m_pQbert->GetSubject()->AddObserver(playerObserver);

	
	InputManager::GetInstance().SetUseKeyboard(true);
	InputManager::GetInstance().AddCommand(SDLK_w, new MoveCommand(ConnectionDirection::upRight, m_pQbert, m_pPyramid));
	InputManager::GetInstance().AddCommand(SDLK_d, new MoveCommand(ConnectionDirection::downRight, m_pQbert, m_pPyramid));
	InputManager::GetInstance().AddCommand(SDLK_s, new MoveCommand(ConnectionDirection::downLeft, m_pQbert, m_pPyramid));
	InputManager::GetInstance().AddCommand(SDLK_a, new MoveCommand(ConnectionDirection::upLeft, m_pQbert, m_pPyramid));
}

void QBertScene::ResetScene(Level newLevel)
{
	m_Level = newLevel;
	m_pPyramid->Reset();
	m_pQbert->Reset(false, m_pPyramid->GetTop());
}

void QBertScene::ResetGame()
{
	m_Level = Level::Level1;
	m_pPyramid->Reset();
	m_pQbert->Reset(true, m_pPyramid->GetTop());
}