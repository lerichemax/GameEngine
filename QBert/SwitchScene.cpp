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

void SwitchScene::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(SwitchScene).name());
	writer.WriteString("sceneName", m_TargetSceneName);
}

void SwitchScene::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadString("sceneName", m_TargetSceneName);
}