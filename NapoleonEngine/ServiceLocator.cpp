#include "PCH.h"
#include "ServiceLocator.h"

using namespace empire;

NullSoundInterface ServiceLocator::m_NullSoundSystem;
SoundInterface* ServiceLocator::m_pSoundSystem = &m_NullSoundSystem;

void ServiceLocator::Register(SoundInterface* pSound)
{
	m_pSoundSystem = pSound == nullptr ? &m_NullSoundSystem : pSound;
}