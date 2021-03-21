#pragma once
#include "Singleton.h"
#include <map>
#include <string>

namespace empire
{
	class SoundInterface 
	{
	public:
		SoundInterface() = default;
		SoundInterface(SoundInterface const& other) = delete;
		SoundInterface(SoundInterface&& other) = delete;
		SoundInterface& operator=(SoundInterface const&  rhs) = delete;
		SoundInterface& operator=(SoundInterface&& rhs) = delete;
		virtual ~SoundInterface() = default;

		virtual void Play(unsigned int id, float volume) = 0;
		virtual void Update() = 0;
	};
}