#include "PCH.h"
#include  "PauseGameCommand.h"

#include "GameObject.h"
#include "InputManager.h"
#include "GameManagerSystem.h"
#include "Timer.h"

PauseGameCommand::PauseGameCommand(GameManagerSystem* const pGameManager)
	:m_pGameManager(pGameManager)
{
}

void PauseGameCommand::Execute(GameObject const& gObject)
{
	m_pGameManager->TogglePause();
}

void PauseGameCommand::Serialize(StreamWriter& writer) const
{

	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(PauseGameCommand)).hash_code()));
	writer.WriteInt("gameManager", m_pGameManager->GetId());
	Command::Serialize(writer);
}

void PauseGameCommand::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	Command::Deserialize(reader, context);
}

void PauseGameCommand::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	int id{-1};
	reader->ReadInt("gameManager", id);
	m_pGameManager =  context.GetRef<GameManagerSystem>(id);
}
