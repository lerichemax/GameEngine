#include "PCH.h"
#include "ReloadSceneCommand.h"

#include "SceneManager.h"

void ReloadSceneCommand::Execute(GameObject* const gObject)
{
	SceneManager::Get().ReloadCurrentScene();
}

void ReloadSceneCommand::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(ReloadSceneCommand)).hash_code()));

	Command::Serialize(writer);
}

void ReloadSceneCommand::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	Command::Deserialize(reader, context);
}