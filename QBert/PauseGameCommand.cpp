#include "PCH.h"
#include  "PauseGameCommand.h"

#include "Timer.h"

PauseGameCommand::PauseGameCommand(KeyActionState keyState)
	:Command(keyState),
	m_bIsPaused(false)
{
	
}

void PauseGameCommand::Execute()
{
	if (m_bIsPaused)
	{
		empire::Timer::GetInstance().SetTimeScale(1);
	}
	else
	{
		empire::Timer::GetInstance().SetTimeScale(0);
	}
	m_bIsPaused = !m_bIsPaused;
}
