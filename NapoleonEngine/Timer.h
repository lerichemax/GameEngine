#pragma once
#include "Singleton.h"
#include <chrono>

#include "NapoleonEngine.h"

namespace empire
{
	class Timer final : public Singleton<Timer>
	{
	public:
		
		float GetDeltaTime() const { return m_DeltaTime; }
		void Update();

		void SetTimeScale(float scale) { m_TimeScale = scale; }
		int GetFPS()const { return m_FPS; }
	private:
		friend class Singleton<Timer>;
		friend void NapoleonEngine::Run();
		
		Timer();

		static const int MS_PER_FRAME = 16; //16 for 60 fps, 33 for 30 fps

		std::chrono::steady_clock::time_point m_LastTime;

		float m_Lag;
		float m_DeltaTime;
		float m_TimeScale;
		int m_FPS;
		void Sleep();
		
	};
}
