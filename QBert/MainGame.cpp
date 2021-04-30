#include "PCH.h"
#include "MainGame.h"
#include "Coily.h"
#include "ColoredDisk.h"
#include "QBertScene.h"
#include "SlickSam.h"
#include "Qube.h"

#include "RendererComponent.h"
#include "PrefabsManager.h"
#include "ResourceManager.h"


using namespace empire;

void MainGame::LoadGame() const
{
	
	SceneManager::GetInstance().AddScene(new QBertScene{});
	SceneManager::GetInstance().SetSceneActive("QBertScene");
}

