#include "PCH.h"
#include "SwitchScene.h"

#include "SceneManager.h"

SwitchScene::SwitchScene(std::string pTargetScene)
	:m_TargetSceneName(pTargetScene)
{
	
}

SwitchScene::SwitchScene(SwitchScene const& other)
	:m_TargetSceneName(other.m_TargetSceneName)
{
	
}

void SwitchScene::Execute()
{
	SceneManager::GetInstance().SetSceneActive(m_TargetSceneName);
}