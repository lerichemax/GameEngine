#include "PCH.h"
#include "MainGame.h"

#include "RendererComponent.h"
#include "ScriptComponent.h"
#include "ColliderComponent.h"

#include "TestScene.h"

#include "PrefabsManager.h"
#include "ResourceManager.h"

MainGame::MainGame()
	:NapoleonEngine{500, 500, "Test", false}
{

}

void MainGame::InitGame() const
{
	SceneManager::Get().AddScene(new TestScene{});
	SceneManager::Get().LoadScene("TestScene");
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

	pPrefabManager.SavePrefab(qbertPrefab, "QBert");
}