#include "PCH.h"
#include "GameScene.h"

#include "PlayerControllerSystem.h"

#include "PrefabsManager.h"

GameScene::GameScene()
	:Scene{"Game Scene"}
{

}

void GameScene::Initialize()
{
	Instantiate("Player");

	AddSystem<PlayerControllerSystem>();
}