#include "PCH.h"
#include "MainMenuScene.h"

#include "GameObject.h"
#include "RendererComponent.h"
#include "ButtonComponent.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"
#include "SceneManager.h"

using namespace empire;
MainMenuScene::MainMenuScene()
	:Scene("MainMenuScene")
{
}

void MainMenuScene::Initialize()
{
	Renderer::GetInstance().SetBackgroundColor(0, 77, 153, 1);


	
	auto titleObject = new GameObject();
	titleObject->AddComponent(new RendererComponent{ "Textures/UI/Title.png", Layer::ui });
	AddObject(titleObject);
	titleObject->GetTransform()->Translate(300, 50);
	titleObject->GetTransform()->Scale(0.8f, 0.8f);

	auto soloBtn = new GameObject();
	auto btn = new ButtonComponent(70, 50);
	btn->SetVisualize(true);
	soloBtn->AddComponent(btn);

	auto textObj = new GameObject();
	auto const font = ResourceManager::GetInstance().GetFont("Fonts/Lingua.otf", 28);
	auto txt = new TextRendererComponent{ "Solo", font };
	textObj->AddComponent(txt);
	textObj->GetTransform()->Translate(7, 10);
	soloBtn->AddChild(textObj);
	
	btn->SetOnSelectFunction([](GameObject* pBtnObject)
		{
			pBtnObject->GetComponentInChildren<TextRendererComponent>()->SetTextColor(255, 0, 0);
		});

	btn->SetOnDeselectFunction([](GameObject* pBtnObject)
		{
			pBtnObject->GetComponentInChildren<TextRendererComponent>()->SetTextColor(255, 255, 255);
		});

	btn->SetOnClickFunction([](GameObject* )
		{
			SceneManager::GetInstance().SetSceneActive("QBertScene");
		});

	soloBtn->GetTransform()->Translate(400, 200);
	AddObject(soloBtn);


	auto coopBtn = new GameObject();
	btn = new ButtonComponent(80, 50);
	btn->SetVisualize(true);
	coopBtn->AddComponent(btn);

	textObj = new GameObject();
	txt = new TextRendererComponent{ "Co-op", font };
	textObj->AddComponent(txt);
	textObj->GetTransform()->Translate(7, 10);
	coopBtn->AddChild(textObj);

	btn->SetOnSelectFunction([](GameObject* pBtnObject)
		{
			pBtnObject->GetComponentInChildren<TextRendererComponent>()->SetTextColor(255, 0, 0);
		});

	btn->SetOnDeselectFunction([](GameObject* pBtnObject)
		{
			pBtnObject->GetComponentInChildren<TextRendererComponent>()->SetTextColor(255, 255, 255);
		});

	btn->SetOnClickFunction([](GameObject*)
		{
			SceneManager::GetInstance().SetSceneActive("CoopScene");
		});

	coopBtn->GetTransform()->Translate(400, 250);
	AddObject(coopBtn);

	auto vsBtn = new GameObject();
	btn = new ButtonComponent(80, 50);
	btn->SetVisualize(true);
	vsBtn->AddComponent(btn);

	textObj = new GameObject();
	txt = new TextRendererComponent{ "Versus", font };
	textObj->AddComponent(txt);
	textObj->GetTransform()->Translate(7, 10);
	vsBtn->AddChild(textObj);

	btn->SetOnSelectFunction([](GameObject* pBtnObject)
		{
			pBtnObject->GetComponentInChildren<TextRendererComponent>()->SetTextColor(255, 0, 0);
		});

	btn->SetOnDeselectFunction([](GameObject* pBtnObject)
		{
			pBtnObject->GetComponentInChildren<TextRendererComponent>()->SetTextColor(255, 255, 255);
		});

	//btn->SetOnClickFunction([](GameObject*)
	//	{
	//		
	//	});

	vsBtn->GetTransform()->Translate(400, 300);
	AddObject(vsBtn);
}