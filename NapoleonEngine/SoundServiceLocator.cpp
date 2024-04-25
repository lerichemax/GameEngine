#include "PCH.h"
#include "SoundServiceLocator.h"

NullSoundInterface SoundServiceLocator::m_NullSoundSystem;
SoundInterface* SoundServiceLocator::m_pSoundSystem = &m_NullSoundSystem;

void SoundServiceLocator::Register(SoundInterface* pSound)
{
	m_pSoundSystem = pSound == nullptr ? &m_NullSoundSystem : pSound;
}