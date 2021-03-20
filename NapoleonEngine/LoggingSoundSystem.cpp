#include "PCH.h"
#include "LoggingSoundSystem.h"

using namespace empire;

LoggingSoundSystem::LoggingSoundSystem(bool isMuted) : SoundSystem(isMuted)
{
	
}

void LoggingSoundSystem::Play(unsigned int id, float volume)
{
	if (!m_bIsMuted)
	{
		SoundSystem::Play(id, volume);
	}

	std::cout << "Playing " << id << " at volume " << volume << std::endl;
}