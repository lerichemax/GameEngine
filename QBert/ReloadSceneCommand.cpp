#include "PCH.h"
#include "ReloadSceneCommand.h"

#include "QBertScene.h"

ReloadSceneCommand::ReloadSceneCommand(QBertScene* pScene)
	:m_pScene(pScene)
{}

ReloadSceneCommand::ReloadSceneCommand(ReloadSceneCommand const& other)
	:m_pScene(other.m_pScene)
{
	
}

void ReloadSceneCommand::Execute()
{
	m_pScene->ResetGame();
}