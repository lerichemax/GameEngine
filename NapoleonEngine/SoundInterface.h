#pragma once
//TO REWORK
class SoundInterface 
{
public:
	virtual ~SoundInterface() = default;
		
	virtual void Play(unsigned int id, float volume) = 0;
	virtual void Update() = 0;
};

class NullSoundInterface final : public SoundInterface
{
public:
	void Play(unsigned int, float) override {};
	void Update() override{};
};