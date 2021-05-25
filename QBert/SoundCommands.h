#pragma once
#include "Command.h"
#include "SoundSystem.h"

namespace empire
{
	class PlaySoundCommand final : public Command
	{
	public:
		PlaySoundCommand(ID id, float volume, KeyActionState state = KeyActionState::pressed);
		
		PlaySoundCommand(PlaySoundCommand const& other) = delete;
		PlaySoundCommand(PlaySoundCommand&& other) = delete;
		PlaySoundCommand& operator=(PlaySoundCommand const& rhs) = delete;
		PlaySoundCommand& operator=(PlaySoundCommand&& rhs) = delete;
		~PlaySoundCommand() override = default;

		void Execute() override;
	private:
		ID m_SoundId;
		float m_Volume;
	};
}