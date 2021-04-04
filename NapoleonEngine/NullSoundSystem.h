#pragma once
#include "SoundSystem.h"

namespace empire
{
	class NullSoundSystem final : public SoundSystem
	{
	public:

		void Play(unsigned int, float) override{};
	};
}
