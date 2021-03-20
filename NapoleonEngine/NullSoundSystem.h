#pragma once
#include "SoundSystem.h"

namespace empire
{
	class NullSoundSystem final : public SoundSystem
	{
	public:
		~NullSoundSystem() override;

		void Play(unsigned int id, float volume) override{};
	};
}
