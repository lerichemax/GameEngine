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

	writer.WriteString("type", typeid(QuitGameCommand).name());
}