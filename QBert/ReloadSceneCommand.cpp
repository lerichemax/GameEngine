#include "PCH.h"
#include "ReloadSceneCommand.h"

#include "SceneManager.h"

ReloadSceneCommand::ReloadSceneCommand(std::string scenName)
	:m_SceneName(scenName)
{}

void ReloadSceneCommand::Execute(GameObject* const gObject)
{
	SceneManager::GetInstance().LoadScene(m_SceneName);
}