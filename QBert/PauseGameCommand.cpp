#include "PCH.h"
#include  "PauseGameCommand.h"

#include "GameObject.h"
#include "InputManager.h"
#include "GameManager.h"
#include "Timer.h"

PauseGameCommand::PauseGameCommand(GameManagerBehaviour* const pGameManager)
	:m_pGameManager(pGameManager)
{
}

void PauseGameCommand::Execute(GameObject* const gObject)
{
	m_pGameManager->TogglePause();
}

void PauseGameCommand::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(PauseGameCommand).name());
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
	m_pGameManager =  context.GetRef<GameManagerBehaviour>(id);
}
