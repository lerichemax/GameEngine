#include "PCH.h"
#include "MainGame.h"

#include "GameScene.h"

#include "RendererComponent.h"
#include "PlayerComponent.h"
#include "ColliderComponent.h"

#include "PrefabsManager.h"
#include "ResourceManager.h"

MainGame::MainGame()
	:NapoleonEngine{600, 400, "Space Invaders", true}
{

}

void MainGame::RegisterScenes() const
{
	SceneManager::Get().AddScene(new GameScene());
	SceneManager::Get().LoadScene("Game Scene");
}

void MainGame::CreatePrefabs() const
{
	auto pProjectilePrefab = PrefabsManager::Get().CreatePrefab();
	auto pProjectileObject = pProjectilePrefab->CreateGameObject();
	pProjectileObject->AddComponent<RendererComponent>()->pTexture = ResourceManager::Get().GetTexture("Projectiles/Projectile_Player.png");
	auto pCollider = pProjectileObject->AddComponent<ColliderComponent>();
	pCollider->SetShape(std::make_unique<geo::Rectangle>(glm::vec2{ 0,0 }, 2, 8, Color{255,0,0}));
	//pCollider->bDraw = true;
	pProjectileObject->GetTransform()->Scale(2.f);

	PrefabsManager::Get().SavePrefab(pProjectilePrefab, "PlayerProjectile");

	auto pPlayerPrefab = PrefabsManager::Get().CreatePrefab();
	auto pPlayerObject = pPlayerPrefab->CreateGameObject();
	auto pRenderer = pPlayerObject->AddComponent<RendererComponent>();
	pRenderer->pTexture = ResourceManager::Get().GetTexture("Player.png");
	pPlayerObject->GetTransform()->SetLocation(0, 150);
	pPlayerObject->GetTransform()->Scale(1.5f);

	pPlayerObject->AddComponent<PlayerComponent>();

	auto pShootTransform = pPlayerPrefab->CreateGameObject();
	pShootTransform->GetTransform()->SetLocalLocation({ 8.5f, -1 });
	//auto pShapeRenderer = pShootTransform->AddComponent<RendererComponent>();
	//pShapeRenderer->SetShape(std::make_unique<Circle>(glm::vec2{ 0,0 }, 2, Color{ 255, 0,0 } ));
	//pShapeRenderer->Layer = 10;
	//pPlayerObject->AddChild(pShootTransform);
	
	PrefabsManager::Get().SavePrefab(pPlayerPrefab, "Player");



}