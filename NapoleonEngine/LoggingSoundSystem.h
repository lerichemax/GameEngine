#pragma once
#include "SoundInterface.h"

namespace empire
{
	class LoggingSoundSystem final : public SoundInterface
	{
	public:
		explicit LoggingSoundSystem(SoundInterface* pRealSS, bool isMuted = false);
		LoggingSoundSystem(LoggingSoundSystem const& other) = delete;
		LoggingSoundSystem(LoggingSoundSystem&& other) = delete;
		LoggingSoundSystem& operator=(LoggingSoundSystem const& rhs) = delete;
		LoggingSoundSystem& operator=(LoggingSoundSystem&& rhs) = delete;
		~LoggingSoundSystem();

		void Play(unsigned int id, float volume) override;
		void Update() override;
	private:
		SoundInterface* m_pRealSoundSystem;
	};
}
