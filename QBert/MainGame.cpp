#include "PCH.h"
#include "MainGame.h"

#include "ServiceLocator.h"
#include "DemoScene.h"
#include "QBertScene.h"

using namespace empire;

void MainGame::LoadGame() const
{
	SceneManager::GetInstance().AddScene(new DemoScene{});
	SceneManager::GetInstance().AddScene(new QBertScene{});
	SceneManager::GetInstance().SetSceneActive("QBertScene");
}