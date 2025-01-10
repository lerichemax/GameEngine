#pragma once
#include "System.h"

class AudioSystem : public ecs::System
{
public:
	AudioSystem() = default;
	AudioSystem(AudioSystem const& other) = default;
	AudioSystem(AudioSystem&& other) = delete;
	AudioSystem& operator=(AudioSystem const& rhs) = delete;
	AudioSystem& operator=(AudioSystem&& rhs) = delete;

	void Update() override;
};