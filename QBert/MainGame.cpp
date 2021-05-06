#include "PCH.h"
#include "MainGame.h"
#include "QBertScene.h"
#include "TestScene.h"

#include "RendererComponent.h"
#include "PrefabsManager.h"
#include "ResourceManager.h"


using namespace empire;

void MainGame::LoadGame() const
{
	//SceneManager::GetInstance().AddScene(new QBertScene{});
	//SceneManager::GetInstance().SetSceneActive("QBertScene");

	SceneManager::GetInstance().AddScene(new TestScene{});
	SceneManager::GetInstance().SetSceneActive("TestScene");
}

