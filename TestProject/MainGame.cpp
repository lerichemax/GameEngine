#include "PCH.h"
#include "MainGame.h"

#include "RendererComponent.h"
#include "ScriptComponent.h"
#include "ColliderComponent.h"

#include "TestScene.h"
#include "2DPlanets.h"

#include "PrefabsManager.h"
#include "ResourceManager.h"

MainGame::MainGame()
	:NapoleonEngine{1000, 1000, "Tests", true}
{

}

void MainGame::RegisterScenes() const
{
	SceneManager::Get().AddScene(new TestScene{});
	SceneManager::Get().AddScene(new Planets{});
	SceneManager::Get().LoadScene("Planets");
}

void MainGame::CreatePrefabs() const
{
	auto& pPrefabManager = PrefabsManager::Get();

	auto qbertPrefab = pPrefabManager.CreatePrefab();
	auto pQbertObj = qbertPrefab->CreateGameObject();

	auto rendererComp = pQbertObj->AddComponent<RendererComponent>();
	rendererComp->Layer = 8;
	rendererComp->pTexture = ResourceManager::Get().GetTexture("Textures/QBert/QBert1_DownLeft_Qube.png");

	auto* pCollider = pQbertObj->AddComponent<ColliderComponent>();
	pCollider->SetShape(new geo::Rectangle{ pQbertObj->GetTransform()->GetLocation(), 24,24, {255, 0, 0} });
	pCollider->bIsTrigger = true;

	pQbertObj->AddComponent<ScriptComponent>()->ScriptFile = "MoveLeft";

	auto pHurtTextObj = qbertPrefab->CreateGameObject();

	auto pHurtRenderer = pHurtTextObj->AddComponent<RendererComponent>();
	pHurtRenderer->pTexture = ResourceManager::Get().GetTexture("Textures/QBert/HurtText.png");
	pHurtRenderer->Layer = 8;

	pQbertObj->AddChild(pHurtTextObj);
	pHurtTextObj->GetTransform()->SetLocalLocation(-10, -34);

	pPrefabManager.SavePrefab(qbertPrefab, "QBert");
}