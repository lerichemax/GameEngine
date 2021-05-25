#include "PCH.h"
#include "SoundCommands.h"
#include "ServiceLocator.h"

using namespace empire;

PlaySoundCommand::PlaySoundCommand(ID id, float volume, KeyActionState state)
	: Command(state),
	m_SoundId(id),
	m_Volume(volume)
{}

void PlaySoundCommand::Execute()
{
	ServiceLocator::GetService().Play(m_SoundId, m_Volume);
}