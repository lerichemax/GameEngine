#include "PCH.h"
#include "TestScene.h"

#include "Component.h"

#include "RendererComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"

#include "ResourceManager.h"
#include "SceneRenderer.h"

TestScene::TestScene()
	:Scene{"Test Scene"}
{
}

void TestScene::Initialize()
{
	auto testObject = CreateGameObject();

	ECS_RendererComponent renderer{};
	renderer.m_pTexture = ResourceManager::GetInstance().GetTexture("Textures/QBert/HurtText.png");
	renderer.m_Layer = 11;

	testObject->AddComponent<ECS_RendererComponent>(renderer);
	testObject->GetTransform()->Translate(450, 300);
}

void TestScene::DeclareInput()
{

}
