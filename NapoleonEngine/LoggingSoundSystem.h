#pragma once
#include "SoundSystem.h"

namespace empire
{
	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		LoggingSoundSystem(bool isMuted = false);
		LoggingSoundSystem(LoggingSoundSystem const& other) = delete;
		LoggingSoundSystem(LoggingSoundSystem&& other) = delete;
		LoggingSoundSystem& operator=(LoggingSoundSystem const& rhs) = delete;
		LoggingSoundSystem& operator=(LoggingSoundSystem&& rhs) = delete;
		~LoggingSoundSystem() = default;

		void Play(unsigned int id, float volume) override;
	};
}
