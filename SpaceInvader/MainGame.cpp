#include "PCH.h"
#include "MainGame.h"

#include "GameScene.h"

#include "RendererComponent.h"
#include "PlayerComponent.h"
#include "ColliderComponent.h"
#include "ProjectileComponent.h"
#include "AnimationComponent.h"
#include "LifeComponent.h"

#include "PrefabsManager.h"
#include "ResourceManager.h"

#include "Animation.h"

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
	pEnemy1Animation->SetAnimation(ResourceManager::Get().RegisterAnimation("Alien1_Idle", "Alien1-1.png", "Alien1-2.png"));
	auto pEnemy1Life = pEnemy1Object->AddComponent<LifeComponent>();
	auto pAlienDeathAnim = ResourceManager::Get().RegisterAnimation("Alien_Death", "Alien_Explosion.png");
	pAlienDeathAnim->m_bLoop = false;
	pEnemy1Life->pDeathAnimation = pAlienDeathAnim;

	auto pEnemy1Collider = pEnemy1Object->AddComponent<ColliderComponent>();
	pEnemy1Collider->SetShape(std::make_unique<geo::Rectangle>(glm::vec2{6.f, 0.f}, 20, 20, Color{ 255,0,0 }));
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
	pEnemy2Animation->SetAnimation(ResourceManager::Get().RegisterAnimation("Alien2_Idle", "Alien2-1.png", "Alien2-2.png"));
	auto pEnemy2Life = pEnemy2Object->AddComponent<LifeComponent>();
	pEnemy2Life->pDeathAnimation = pAlienDeathAnim;

	auto pEnemy2Collider = pEnemy2Object->AddComponent<ColliderComponent>();
	pEnemy2Collider->SetShape(std::make_unique<geo::Rectangle>(glm::vec2{ 6.f, 0.f }, 20, 20, Color{ 255,0,0 }));
	pEnemy2Collider->bIsTrigger = true;

	pEnemy2Object->GetTransform()->Scale(2.f);
	pEnemy2Object->SetTag("Enemy");

	pPrefabManager->SavePrefab(pEnemy2Prefab, "Enemy2");

	//enemy 3
	auto pEnemy3Prefab = pPrefabManager->CreatePrefab();
	auto pEnemy3Object = pEnemy3Prefab->CreateGameObject();
	auto pEnemy3Renderer = pEnemy3Object->AddComponent<RendererComponent>();
	pEnemy3Renderer->pTexture = ResourceManager::Get().GetTexture("Alien3-1.png");
	auto pEnemy3Animation = pEnemy3Object->AddComponent<AnimationComponent>();
	pEnemy3Animation->SetAnimation(ResourceManager::Get().RegisterAnimation("Alien3_Idle", "Alien3-1.png", "Alien3-2.png"));
	auto pEnemy3Life = pEnemy3Object->AddComponent<LifeComponent>();
	pEnemy3Life->pDeathAnimation = pAlienDeathAnim;

	auto pEnemy3Collider = pEnemy3Object->AddComponent<ColliderComponent>();
	pEnemy3Collider->SetShape(std::make_unique<geo::Rectangle>(glm::vec2{ 6.f, 0.f }, 20, 20, Color{ 255,0,0 }));
	pEnemy3Collider->bIsTrigger = true;

	pEnemy3Object->GetTransform()->Scale(2.f);
	pEnemy3Object->SetTag("Enemy");

	pPrefabManager->SavePrefab(pEnemy3Prefab, "Enemy3");

}