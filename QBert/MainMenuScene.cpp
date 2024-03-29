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

using namespace empire;
MainMenuScene::MainMenuScene()
	:Scene("MainMenuScene")
{
}

void MainMenuScene::Initialize()
{
	
	auto* titleObject = new GameObject();
	titleObject->AddComponent(new RendererComponent{ "Textures/UI/Title.png", Layer::uiGame });
	AddObject(titleObject);
	titleObject->GetTransform()->Translate(300, 50);
	titleObject->GetTransform()->Scale(0.8f, 0.8f);

	auto* soloBtn = new GameObject();
	auto* btn = new ButtonComponent(70, 30);
	soloBtn->AddComponent(btn);

	auto* textObj = new GameObject();
	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 28);
	auto txt = new TextRendererComponent{ "Solo", font };
	textObj->AddComponent(txt);
	textObj->GetTransform()->Translate(7, 3);
	soloBtn->AddChild(textObj);

	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, txt });

	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, txt });

	btn->SetOnClickFunction(new SwitchScene{"SoloScene"});

	soloBtn->GetTransform()->Translate(400, 200);
	AddObject(soloBtn);

	auto* coopBtn = new GameObject();
	btn = new ButtonComponent(80, 30);
	coopBtn->AddComponent(btn);

	textObj = new GameObject();
	txt = new TextRendererComponent{ "Co-op", font };
	textObj->AddComponent(txt);
	textObj->GetTransform()->Translate(7, 3);
	coopBtn->AddChild(textObj);

	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, txt });

	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, txt });

	btn->SetOnClickFunction(new SwitchScene{"CoopScene"});

	coopBtn->GetTransform()->Translate(400, 250);
	AddObject(coopBtn);

	auto* vsBtn = new GameObject();
	btn = new ButtonComponent(92, 30);
	vsBtn->AddComponent(btn);

	textObj = new GameObject();
	txt = new TextRendererComponent{ "Versus", font };
	textObj->AddComponent(txt);
	textObj->GetTransform()->Translate(7, 3);
	vsBtn->AddChild(textObj);

	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, txt });

	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, txt });

	btn->SetOnClickFunction(new SwitchScene{"VersusScene"});

	vsBtn->GetTransform()->Translate(400, 300);
	AddObject(vsBtn);

	auto* quitBtn = new GameObject();
	btn = new ButtonComponent(70, 30);
	quitBtn->AddComponent(btn);

	textObj = new GameObject();
	txt = new TextRendererComponent{ "Quit", font };
	textObj->AddComponent(txt);
	textObj->GetTransform()->Translate(7, 3);
	quitBtn->AddChild(textObj);

	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, txt });

	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, txt });

	btn->SetOnClickFunction(new QuitGameCommand{  });

	quitBtn->GetTransform()->Translate(400, 350);
	AddObject(quitBtn);
	
	auto* camObj = new GameObject{};
	auto* camComp = new CameraComponent{};
	camObj->AddComponent(camComp);
	camObj->GetTransform()->Translate(450, 300);
	SetCameraActive(camComp);
	AddObject(camObj);
}

void MainMenuScene::CustomOnActivate()
{
	Renderer::GetInstance().SetBackgroundColor(0, 77, 153, 1);
}