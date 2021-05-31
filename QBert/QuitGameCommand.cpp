#include "PCH.h"
#include "QuitGameCommand.h"


#include "NapoleonEngine.h"

void QuitGameCommand::Execute()
{
	NapoleonEngine::Quit();
}
