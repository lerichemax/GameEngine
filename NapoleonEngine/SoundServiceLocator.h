#pragma once
#include "SoundSystem.h"

class SoundServiceLocator final
{
public:
	static SoundInterface& GetService() { return *m_pSoundSystem; }
	static void Register(SoundInterface* pSound); 

private:
	static SoundInterface* m_pSoundSystem;
	static NullSoundInterface m_NullSoundSystem;
};
