#include "MiniginPCH.h"
#include "QuitCommand.h"

bool QuitCommand::Execute()
{
	std::cout << "Quitting";
	return false;
}