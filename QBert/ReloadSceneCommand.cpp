#include "PCH.h"
#include "ReloadSceneCommand.h"

#include "SceneManager.h"

void ReloadSceneCommand::Execute(GameObject const& gObject)
{
	SceneManager::Get().ReloadCurrentScene();
}
