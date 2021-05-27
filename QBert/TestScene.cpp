#include "PCH.h"
#include "TestScene.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "WrongWay.h"

void TestScene::Initialize()
{
	auto go = new empire::GameObject{};
	go->AddComponent(new empire::RendererComponent{ "Textures/Enemies/WrongWay/WrongWay.png" , empire::Layer::middleground});
	go->AddComponent(new WrongWay(true));

	go->GetTransform()->Translate(350, 350);
	go->GetTransform()->Scale(2.f, 2.f);
	AddObject(go);
}