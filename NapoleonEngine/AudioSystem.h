#pragma once
#include "System.h"
#include "SoundInterface.h"
#include "AudioComponent.h"

#include <map>
#include <string>
#include <mutex>

using ID = unsigned int;

class SoundEffect;
class Registry;
class AudioSystem : public System
{
public:
	AudioSystem() = default;
	AudioSystem(AudioSystem const& other) = default;
	AudioSystem(AudioSystem&& other) = delete;
	AudioSystem& operator=(AudioSystem const& rhs) = delete;
	AudioSystem& operator=(AudioSystem&& rhs) = delete;

	void Update() override;

	void SetSignature() const override;
};