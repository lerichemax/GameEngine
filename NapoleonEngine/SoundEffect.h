#pragma once
#include <string>

#include "AudioSystem.h"

struct Mix_Chunk;
class SoundEffect final
{
	friend class AudioSystem;
public:
	explicit SoundEffect(std::string const& path);
	SoundEffect(SoundEffect const& other) = delete;
	SoundEffect(SoundEffect&& other) = delete;
	SoundEffect& operator=(SoundEffect const& rhs) = delete;
	SoundEffect& operator=(SoundEffect&& rhs) = delete;
	~SoundEffect();

	bool Play();
	bool Play(float volume);

	void SetVolume(float volume);
	ID GetId() const { return m_Id; }
private:
	static ID m_AutoIncrementId;

	ID m_Id;
	Mix_Chunk* m_pMixChunk;

	float m_Volume;
	float m_SavedVolume;
};