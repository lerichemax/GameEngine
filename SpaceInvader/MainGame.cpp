#include "PCH.h"
#include "MainGame.h"

#include "GameScene.h"

#include "RendererComponent.h"
#include "PlayerComponent.h"
#include "ColliderComponent.h"
#include "ProjectileComponent.h"
#include "AnimationComponent.h"

#include "PrefabsManager.h"
#include "ResourceManager.h"

MainGame::MainGame()
	:NapoleonEngine{400, 500, "Space Invaders", true}
{

}

void MainGame::RegisterScenes() const
{
	SceneManager::Get().AddScene(new GameScene());
	SceneManager::Get().LoadScene("Game Scene");
}

void MainGame::CreatePrefabs(std::shared_ptr<PrefabsManager> pPrefabManager) const
{
	//player projectile
	auto pProjectilePrefab = pPrefabManager->CreatePrefab();
	auto pProjectileObject = pProjectilePrefab->CreateGameObject();
	pProjectileObject->AddComponent<RendererComponent>()->pTexture = ResourceManager::Get().GetTexture("Projectiles/Projectile_Player.png");
	auto pCollider = pProjectileObject->AddComponent<ColliderComponent>();
	pCollider->bIsTrigger = true;
	pCollider->SetShape(std::make_unique<geo::Rectangle>(glm::vec2{ 0,0 }, 2, 8, Color{255,0,0}));
	pCollider->bDraw = true;
	pProjectileObject->GetTransform()->Scale(2.f);

	auto pProjectileComponent = pProjectileObject->AddComponent<ProjectileComponent>();
	pProjectileComponent->MoveSpeed = 300.f;
	pProjectileComponent->MoveDirection = -1;

	pProjectileObject->SetTag("Projectile");
	pPrefabManager->SavePrefab(pProjectilePrefab, "PlayerProjectile");

	//player
	auto pPlayerPrefab = pPrefabManager->CreatePrefab();
	auto pPlayerObject = pPlayerPrefab->CreateGameObject();
	auto pRenderer = pPlayerObject->AddComponent<RendererComponent>();
	pRenderer->pTexture = ResourceManager::Get().GetTexture("Player.png");
	pPlayerObject->GetTransform()->SetLocation(0, 150);
	pPlayerObject->GetTransform()->Scale(1.5f);

	pPlayerObject->AddComponent<PlayerComponent>();

	auto pShootTransform = pPlayerPrefab->CreateGameObject();
	pShootTransform->GetTransform()->SetLocalLocation({ 8.5f, -1 });
	pPlayerObject->AddChild(pShootTransform);
	
	pPrefabManager->SavePrefab(pPlayerPrefab, "Player");


	//ENEMIES
	//enemy 1
	auto pEnemy1Prefab = pPrefabManager->CreatePrefab();
	auto pEnemy1Object = pEnemy1Prefab->CreateGameObject();
	auto pEnemy1Renderer = pEnemy1Object->AddComponent<RendererComponent>();
	pEnemy1Renderer->pTexture = ResourceManager::Get().GetTexture("Alien1-1.png");
	auto pEnemy1Animation = pEnemy1Object->AddComponent<AnimationComponent>();
	pEnemy1Animation->AnimationSprites.push_back("Alien1-1.png");
	pEnemy1Animation->AnimationSprites.push_back("Alien1-2.png");
	pEnemy1Animation->TimePerSprite = 0.25f;

	auto pEnemy1Collider = pEnemy1Object->AddComponent<ColliderComponent>();
	pEnemy1Collider->SetShape(std::make_unique<geo::Rectangle>(glm::vec2{6.f, 0.f}, 20, 20, Color{ 255,0,0 }));
	pEnemy1Collider->bDraw = true;
	pEnemy1Collider->bIsTrigger = true;

	pEnemy1Object->GetTransform()->Scale(2.f);
	pEnemy1Object->SetTag("Enemy");

	pPrefabManager->SavePrefab(pEnemy1Prefab, "Enemy1");

	//enemy 2
	auto pEnemy2Prefab = pPrefabManager->CreatePrefab();
	auto pEnemy2Object = pEnemy2Prefab->CreateGameObject();
	auto pEnemy2Renderer = pEnemy2Object->AddComponent<RendererComponent>();
	pEnemy2Renderer->pTexture = ResourceManager::Get().GetTexture("Alien2-1.png");
	auto pEnemy2Animation = pEnemy2Object->AddComponent<AnimationComponent>();
	pEnemy2Animation->AnimationSprites.push_back("Alien2-1.png");
	pEnemy2Animation->AnimationSprites.push_back("Alien2-2.png");
	pEnemy2Animation->TimePerSprite = 0.25f;

	auto pEnemy2Collider = pEnemy2Object->AddComponent<ColliderComponent>();
	pEnemy2Collider->SetShape(std::make_unique<geo::Rectangle>(glm::vec2{ 6.f, 0.f }, 20, 20, Color{ 255,0,0 }));
	pEnemy2Collider->bDraw = true;
	pEnemy2Collider->bIsTrigger = true;

	pEnemy2Object->GetTransform()->Scale(2.f);
	pEnemy2Object->SetTag("Enemy");

	pPrefabManager->SavePrefab(pEnemy2Prefab, "Enemy2");

}