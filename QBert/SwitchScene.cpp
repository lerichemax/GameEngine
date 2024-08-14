#include "PCH.h"
#include "SwitchScene.h"

#include "SceneManager.h"

SwitchScene::SwitchScene(std::string pTargetScene)
	:m_TargetSceneName(pTargetScene)
{
	
}

void SwitchScene::Execute(GameObject* const gObject)
{
	SceneManager::GetInstance().SetSceneActive(m_TargetSceneName);
}

void SwitchScene::Serialize(StreamWriter& writer) const
{
	Command::Serialize(writer);

	writer.WriteString("type", typeid(SwitchScene).name());
	writer.WriteString("sceneName", m_TargetSceneName);
}

void SwitchScene::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	Command::Deserialize(reader, context);

	reader->ReadString("sceneName", m_TargetSceneName);
}