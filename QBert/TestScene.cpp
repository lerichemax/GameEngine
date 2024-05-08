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
	GameObject* testObject = CreateGameObject();

	ECS_RendererComponent renderer{};
	renderer.m_pTexture = ResourceManager::GetInstance().GetTexture("Textures/QBert/HurtText.png");
	renderer.m_Layer = Layer::foreground;

	testObject->AddComponent<ECS_RendererComponent>(renderer, true);
	testObject->GetECSTransform()->m_Position = { 450, 300 };

	auto* camObj = CreateGameObject();
	ECS_CameraComponent camComp{};
	camObj->AddComponent<ECS_CameraComponent>(camComp, true);
	camObj->GetECSTransform()->m_Position = {450, 300};
	SetActiveCamera(camObj->GetEntity());
}

void TestScene::DeclareInput()
{

}
