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

MainMenuScene::MainMenuScene()
	:Scene("MainMenuScene")
{
}

void MainMenuScene::Initialize()
{
	ECS_RendererComponent rendererComp;
	rendererComp.m_pTexture = ResourceManager::GetInstance().GetTexture("Textures/UI/Title.png");
	rendererComp.m_Layer = Layer::uiGame;

	GameObject* titleObject = CreateGameObject();
	titleObject->AddComponent<ECS_RendererComponent>(rendererComp, true);
	titleObject->GetECSTransform()->Translate(300, 50);
	titleObject->GetECSTransform()->Scale(0.8f);

	GameObject* soloBtn = CreateGameObject();
	ECS_ButtonComponent btn(70, 30);
	soloBtn->AddComponent<ECS_ButtonComponent>(btn, true);

	ECS_TextRendererComponent txt{ "Solo", ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 28) };

	ECS_RendererComponent txtRendererComp;
	txtRendererComp.m_Layer = Layer::uiGame;

	soloBtn->AddComponent<ECS_TextRendererComponent>(txt, true);
	soloBtn->AddComponent<ECS_RendererComponent>(txtRendererComp, true);
	soloBtn->GetECSTransform()->Translate(7, 3);

	//btn.SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, txt });
	//btn.SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, txt });
	//btn.SetOnClickFunction(new SwitchScene{"SoloScene"});

	soloBtn->GetECSTransform()->Translate(400, 200);

	auto* coopBtn = CreateGameObject();
	btn.m_Dimensions = { 80, 30 };
	coopBtn->AddComponent<ECS_ButtonComponent>(btn, true);

	txt.m_Text = "Co-op";

	coopBtn->AddComponent<ECS_TextRendererComponent>(txt, true);
	coopBtn->AddComponent<ECS_RendererComponent>(txtRendererComp, true);
	coopBtn->GetECSTransform()->Translate(7, 3);

	//btn.SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, txt });
	//btn.SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, txt });
	//btn.SetOnClickFunction(new SwitchScene{"CoopScene"});

	coopBtn->GetECSTransform()->Translate(400, 250);

	auto* vsBtn = CreateGameObject();
	btn.m_Dimensions = { 92, 30 };
	vsBtn->AddComponent<ECS_ButtonComponent>(btn, true);

	txt.m_Text =  "Versus";
	vsBtn->AddComponent<ECS_TextRendererComponent>(txt, true);
	vsBtn->AddComponent<ECS_RendererComponent>(txtRendererComp, true);
	vsBtn->GetECSTransform()->Translate(7, 3);

	//btn.SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, txt });
	//btn.SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, txt });
	//btn.SetOnClickFunction(new SwitchScene{"VersusScene"});

	vsBtn->GetECSTransform()->Translate(400, 300);

	auto* quitBtn = CreateGameObject();
	btn.m_Dimensions = { 70, 30 };
	quitBtn->AddComponent<ECS_ButtonComponent>(btn, true);

	txt.m_Text = "Quit";
	quitBtn->AddComponent<ECS_TextRendererComponent>(txt, true);
	quitBtn->AddComponent<ECS_RendererComponent>(txtRendererComp, true);
	quitBtn->GetECSTransform()->Translate(7, 3);

	//btn.SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, txt });
	//btn.SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, txt });
	//btn.SetOnClickFunction(new QuitGameCommand{  });

	quitBtn->GetECSTransform()->Translate(400, 350);
	
	auto* camObj = CreateGameObject();
	ECS_CameraComponent camComp{};
	camObj->AddComponent<ECS_CameraComponent>(camComp, true);
	camObj->GetECSTransform()->m_Position = { 450, 300 };
	SetActiveCamera(camObj->GetEntity());
}

void MainMenuScene::CustomOnActivate()
{
	Renderer::GetInstance().SetBackgroundColor(0, 77, 153, 1);
}