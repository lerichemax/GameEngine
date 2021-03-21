#pragma once
#include <map>
#include <string>

#include "SoundInterface.h"

namespace empire
{
	using ID = unsigned int;

	struct ToPlay
	{
		ID id;
		float volume;
	};

	class SoundEffect;
	class SoundSystem : public SoundInterface
	{
	public:
		SoundSystem(bool isMuted = false);
		SoundSystem(SoundSystem const& other) = delete;
		SoundSystem(SoundSystem&& other) = delete;
		SoundSystem& operator=(SoundSystem const& rhs) = delete;
		SoundSystem& operator=(SoundSystem&& rhs) = delete;
		virtual ~SoundSystem();

		void Play(unsigned int id, float volume) override;
		void Update() override;

		ID AddEffect(SoundEffect* pEffect);
		ID AddEffect(std::string const& path);

		void Mute();
		void UnMute();

	protected:
		bool m_bIsMuted;

	private:
		static unsigned int const MAX_PENDING{ 16 };

		ToPlay m_Pending[MAX_PENDING];
		unsigned int m_NbrPending;

		std::map<ID, SoundEffect*> m_pSounds;
	};
}