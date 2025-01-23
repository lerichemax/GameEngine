#include "PCH.h"
#include "GameScene.h"

#include "PlayerControllerSystem.h"
#include "ProjectileSystem.h"
#include "AnimationSystem.h"

#include "PrefabsManager.h"

GameScene::GameScene()
	:Scene{"Game Scene"}
{

}

void GameScene::Initialize()
{
	Instantiate("Player");
	Instantiate("Enemy1");

	AddSystem<PlayerControllerSystem>();
	AddSystem<ProjectileSystem>();
	AddSystem<AnimationSystem>();
}