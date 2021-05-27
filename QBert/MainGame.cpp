#include "PCH.h"
#include "MainGame.h"
#include "QBertScene.h"
#include "TestScene.h"
#include "MainMenuScene.h"

#include "RendererComponent.h"
#include "PrefabsManager.h"
#include "ResourceManager.h"


using namespace empire;

void MainGame::LoadGame() const
{
	SceneManager::GetInstance().AddScene(new MainMenuScene{});
	SceneManager::GetInstance().AddScene(new QBertScene{});
	SceneManager::GetInstance().SetSceneActive("MainMenuScene");

	//SceneManager::GetInstance().AddScene(new TestScene{});
	//SceneManager::GetInstance().SetSceneActive("TestScene");
}

