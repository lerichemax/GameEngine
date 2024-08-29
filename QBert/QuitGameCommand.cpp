#include "PCH.h"
#include "QuitGameCommand.h"

#include "NapoleonEngine.h"

void QuitGameCommand::Execute(GameObject* const gObject)
{
	NapoleonEngine::Quit();
}

void QuitGameCommand::Serialize(StreamWriter& writer) const
{
	Command::Serialize(writer);

	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(QuitGameCommand)).hash_code()));
}