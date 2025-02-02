#include "PCH.h"
#include "GameScene.h"

#include "PlayerControllerSystem.h"
#include "ProjectileSystem.h"
#include "AnimationSystem.h"
#include "EnemySystem.h"
#include "LifeSystem.h"

#include "EnemyInfoComponent.h"
#include "EnemyGroupComponent.h"

#include "PrefabsManager.h"

GameScene::GameScene()
	:Scene{"Game Scene"}
{

}

void GameScene::Initialize()
{
	Instantiate("Player");

	auto pEnemyInfoObj = CreateGameObject();
	pEnemyInfoObj->AddComponent<EnemyGroupComponent>();
	auto pEnemyInfo = pEnemyInfoObj->AddComponent<EnemyInfoComponent>();
	pEnemyInfo->EnemyPrefab = "Enemy1";

	pEnemyInfo = pEnemyInfoObj->AddComponent<EnemyInfoComponent>();
	pEnemyInfo->EnemyPrefab = "Enemy2";
	pEnemyInfo->Rows = 2;

	pEnemyInfo = pEnemyInfoObj->AddComponent<EnemyInfoComponent>();
	pEnemyInfo->EnemyPrefab = "Enemy3";
	pEnemyInfo->Rows = 1;

	AddSystem<PlayerControllerSystem>();
	AddSystem<ProjectileSystem>();
	AddSystem<AnimationSystem>();
	AddSystem<EnemySystem>();
	AddSystem<LifeSystem>();
}