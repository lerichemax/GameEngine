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
	std::shared_ptr<GameObject> titleObject = CreateGameObject();
	auto rendererComp = titleObject->AddComponent<ECS_RendererComponent>();
	rendererComp->m_pTexture = ResourceManager::GetInstance().GetTexture("Textures/UI/Title.png");
	rendererComp->m_Layer = 10;

	titleObject->GetTransform()->Translate(300, 50);
	titleObject->GetTransform()->Scale(0.8f);

	std::shared_ptr<GameObject> soloBtn = CreateGameObject();

	auto txt = soloBtn->AddComponent<ECS_TextRendererComponent>();
	txt->m_Text = "Solo";
	txt->m_pFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 28);
	soloBtn->AddComponent<ECS_RendererComponent>()->m_Layer = 10;

	auto btn = soloBtn->AddComponent<ECS_ButtonComponent>();
	btn->m_Dimensions = { 70,30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, soloBtn->GetComponent<ECS_TextRendererComponent>()});
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, soloBtn->GetComponent<ECS_TextRendererComponent>() });
	btn->SetOnClickFunction(new SwitchScene{"SoloScene"});

	soloBtn->GetTransform()->Translate(400, 200);

	auto coopBtn = CreateGameObject();

	txt = coopBtn->AddComponent<ECS_TextRendererComponent>();
	txt->m_Text = "Co-op";
	txt->m_pFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 28);

	coopBtn->AddComponent<ECS_RendererComponent>()->m_Layer = 10;

	btn = coopBtn->AddComponent<ECS_ButtonComponent>();
	btn->m_Dimensions = { 80, 30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, coopBtn->GetComponent<ECS_TextRendererComponent>() });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, coopBtn->GetComponent<ECS_TextRendererComponent>() });
	btn->SetOnClickFunction(new SwitchScene{"CoopScene"});

	coopBtn->GetTransform()->Translate(400, 250);

	auto vsBtn = CreateGameObject();
	
	txt = vsBtn->AddComponent<ECS_TextRendererComponent>();
	txt->m_Text = "Versus";
	txt->m_pFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 28);

	vsBtn->AddComponent<ECS_RendererComponent>()->m_Layer = 10;

	btn = vsBtn->AddComponent<ECS_ButtonComponent>();
	btn->m_Dimensions = { 92, 30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, vsBtn->GetComponent<ECS_TextRendererComponent>() });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, vsBtn->GetComponent<ECS_TextRendererComponent>() });
	btn->SetOnClickFunction(new SwitchScene{"VersusScene"});

	vsBtn->GetTransform()->Translate(400, 300);

	auto quitBtn = CreateGameObject();

	txt = quitBtn->AddComponent<ECS_TextRendererComponent>();
	txt->m_Text = "Quit";
	txt->m_pFont = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 28);

	quitBtn->AddComponent<ECS_RendererComponent>()->m_Layer = 10;

	btn = quitBtn->AddComponent<ECS_ButtonComponent>();
	btn->m_Dimensions = { 70, 30 };
	btn->SetOnSelectFunction(new SwitchTextColor{ Color{255,0,0}, quitBtn->GetComponent<ECS_TextRendererComponent>() });
	btn->SetOnDeselectFunction(new SwitchTextColor{ Color{255,255,255}, quitBtn->GetComponent<ECS_TextRendererComponent>() });
	btn->SetOnClickFunction(new QuitGameCommand{  });

	quitBtn->GetTransform()->Translate(400, 350);

	Instantiate("FPSCounter");

	GetCameraObject()->GetComponent<ECS_TransformComponent>()->Translate(450, 300);
}

void MainMenuScene::CustomOnActivate()
{
	Renderer::GetInstance().SetBackgroundColor(0, 77, 153, 1);
}