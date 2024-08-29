#include "PCH.h"
#include "MainMenuScene.h"

#include "GameObject.h"
#include "RendererComponent.h"
#include "ButtonComponent.h"
#include "QuitGameCommand.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"
#include "SwitchScene.h"
#include "SwitchTextColor.h"
#include "CameraComponent.h"
#include "UiSystem.h"

MainMenuScene::MainMenuScene()
	:Scene("MainMenuScene")
{
}

void MainMenuScene::Initialize()
{
	m_BackgroundColor = { 0, 77, 153, 1 };

	GameObject* const titleObject = CreateGameObject();
	auto rendererComp = titleObject->AddComponent<RendererComponent>();
	rendererComp->pTexture = ResourceManager::GetInstance().GetTexture("Textures/UI/Title.png");
	rendererComp->Layer = 10;

	titleObject->GetTransform()->Translate(300, 50);
	titleObject->GetTransform()->Scale(0.8f);

	GameObject* const soloBtn = CreateGameObject();

	auto txt = soloBtn->AddComponent<TextRendererComponent>();
	txt->SetText("Solo");
	txt->SetFont(ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 28));
	soloBtn->AddComponent<RendererComponent>()->Layer = 10;

	auto btn = soloBtn->AddComponent<ButtonComponent>();
	btn->m_Dimensions = { 70,30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}});
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}});
	btn->SetOnClickFunction(new SwitchScene{"SoloScene"});

	soloBtn->GetTransform()->Translate(400, 200);

	auto coopBtn = CreateGameObject();

	txt = coopBtn->AddComponent<TextRendererComponent>();
	txt->SetText("Co-op");
	txt->SetFont(ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 28));

	coopBtn->AddComponent<RendererComponent>()->Layer = 10;

	btn = coopBtn->AddComponent<ButtonComponent>();
	btn->m_Dimensions = { 80, 30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}});
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	btn->SetOnClickFunction(new SwitchScene{"CoopScene"});

	coopBtn->GetTransform()->Translate(400, 250);

	auto vsBtn = CreateGameObject();
	
	txt = vsBtn->AddComponent<TextRendererComponent>();
	txt->SetText("Versus");
	txt->SetFont(ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 28));

	vsBtn->AddComponent<RendererComponent>()->Layer = 10;

	btn = vsBtn->AddComponent<ButtonComponent>();
	btn->m_Dimensions = { 92, 30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0} });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}});
	btn->SetOnClickFunction(new SwitchScene{"VersusScene"});

	vsBtn->GetTransform()->Translate(400, 300);

	auto quitBtn = CreateGameObject();

	txt = quitBtn->AddComponent<TextRendererComponent>();
	txt->SetText("Quit");
	txt->SetFont(ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 28));

	quitBtn->AddComponent<RendererComponent>()->Layer = 10;

	btn = quitBtn->AddComponent<ButtonComponent>();
	btn->m_Dimensions = { 70, 30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0} });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	btn->SetOnClickFunction(new QuitGameCommand{  });

	quitBtn->GetTransform()->Translate(400, 350);

	Instantiate("FPSCounter");

	GetCameraObject()->GetComponent<ECS_TransformComponent>()->Translate(450, 300);
}