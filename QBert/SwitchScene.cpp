#include "PCH.h"
#include "SwitchScene.h"

#include "SceneManager.h"

SwitchScene::SwitchScene(std::string pTargetScene)
	:m_TargetSceneName(pTargetScene)
{
	
}

void SwitchScene::Execute(GameObject const& gObject)
{
	SceneManager::Get().LoadScene(m_TargetSceneName);
}
