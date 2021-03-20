#pragma once
#include "Command.h"
#include "SoundSystem.h"
#include "ServiceLocator.h"

namespace empire
{
	template <typename T>
	class PlaySoundCommand final : public Command
	{
	public:
		PlaySoundCommand(ID id, float volume, KeyActionState state = KeyActionState::pressed);
		PlaySoundCommand(PlaySoundCommand const& other) = delete;
		PlaySoundCommand(PlaySoundCommand&& other) = delete;
		PlaySoundCommand& operator=(PlaySoundCommand const& rhs) = delete;
		PlaySoundCommand& operator=(PlaySoundCommand&& rhs) = delete;
		~PlaySoundCommand() override = default;

		bool Execute() override;

	private:
		ID m_SoundId;
		float m_Volume;
	};
}

template <typename T>
empire::PlaySoundCommand<T>::PlaySoundCommand(ID id, float volume, KeyActionState state)
	: Command(state),
	m_SoundId(id),
	m_Volume(volume)
{
}

template <typename T>
bool empire::PlaySoundCommand<T>::Execute()
{
	auto& sound = ServiceLocator<T>::GetService();

	sound.Play(m_SoundId, m_Volume);
	return true;
}