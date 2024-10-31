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
#include "UiSystem.h"

MainMenuScene::MainMenuScene()
	:Scene("MainMenuScene")
{
}

void MainMenuScene::Initialize()
{
	m_BackgroundColor = { 0, 77, 153, 1 };

	auto pTitleObject = CreateGameObject();
	auto rendererComp = pTitleObject->AddComponent<RendererComponent>();
	rendererComp->pTexture = ResourceManager::Get().GetTexture("Textures/UI/Title.png");
	rendererComp->Layer = 10;

	pTitleObject->GetTransform()->SetLocation(300, 50);
	pTitleObject->GetTransform()->Scale(0.8f);

	auto pSoloBtn = CreateGameObject();

	auto txt = pSoloBtn->AddComponent<TextRendererComponent>();
	txt->SetText("Solo");
	txt->SetFont(ResourceManager::Get().GetFont("Fonts/Lingua.otf", 28));
	pSoloBtn->AddComponent<RendererComponent>()->Layer = 10;

	auto btn = pSoloBtn->AddComponent<ButtonComponent>();
	btn->Dimensions = { 70,30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}});
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}});
	btn->SetOnClickFunction(new SwitchScene{"SoloScene"});

	pSoloBtn->GetTransform()->SetLocation(400, 200);

	auto coopBtn = CreateGameObject();

	txt = coopBtn->AddComponent<TextRendererComponent>();
	txt->SetText("Co-op");
	txt->SetFont(ResourceManager::Get().GetFont("Fonts/Lingua.otf", 28));

	coopBtn->AddComponent<RendererComponent>()->Layer = 10;

	btn = coopBtn->AddComponent<ButtonComponent>();
	btn->Dimensions = { 80, 30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}});
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	btn->SetOnClickFunction(new SwitchScene{"CoopScene"});

	coopBtn->GetTransform()->SetLocation(400, 250);

	auto vsBtn = CreateGameObject();
	
	txt = vsBtn->AddComponent<TextRendererComponent>();
	txt->SetText("Versus");
	txt->SetFont(ResourceManager::Get().GetFont("Fonts/Lingua.otf", 28));

	vsBtn->AddComponent<RendererComponent>()->Layer = 10;

	btn = vsBtn->AddComponent<ButtonComponent>();
	btn->Dimensions = { 92, 30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0} });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}});
	btn->SetOnClickFunction(new SwitchScene{"VersusScene"});

	vsBtn->GetTransform()->SetLocation(400, 300);

	auto quitBtn = CreateGameObject();

	txt = quitBtn->AddComponent<TextRendererComponent>();
	txt->SetText("Quit");
	txt->SetFont(ResourceManager::Get().GetFont("Fonts/Lingua.otf", 28));

	quitBtn->AddComponent<RendererComponent>()->Layer = 10;

	btn = quitBtn->AddComponent<ButtonComponent>();
	btn->Dimensions = { 70, 30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0} });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255} });
	btn->SetOnClickFunction(new QuitGameCommand{  });

	quitBtn->GetTransform()->SetLocation(400, 350);

	Instantiate("FPSCounter");
}