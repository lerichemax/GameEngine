#pragma once
//TO REWORK
class SoundInterface 
{
public:
	virtual ~SoundInterface() = default;
		
	virtual void Play(unsigned int id, float volume) = 0;
	virtual void Update() = 0;

	void Mute() { m_bIsMuted = true; };
	void UnMute() { m_bIsMuted = false; };

protected:
	bool m_bIsMuted{};

};

class NullSoundInterface final : public SoundInterface
{
public:
	void Play(unsigned int, float) override {};
	void Update() override{};
};