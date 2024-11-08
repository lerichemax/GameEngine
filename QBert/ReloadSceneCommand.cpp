#include "PCH.h"
#include "ReloadSceneCommand.h"

#include "SceneManager.h"

void ReloadSceneCommand::Execute(GameObject const& gObject)
{
	SceneManager::Get().ReloadCurrentScene();
}

void ReloadSceneCommand::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(ReloadSceneCommand).name());

	Command::Serialize(writer);
}

void ReloadSceneCommand::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	Command::Deserialize(reader, context);
}