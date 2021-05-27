#include "PCH.h"
#include "Timer.h"

#include <thread>

using namespace empire;
using namespace std::chrono;

Timer::Timer()
	:Singleton<Timer>(),
	m_DeltaTime(0.f),
	m_Lag(0.f),
	m_TimeScale(1.f),
	m_LastTime(high_resolution_clock::now())
{
}

void Timer::Update()
{
	const auto currentTime = high_resolution_clock::now();
	m_DeltaTime =  duration<float>(currentTime - m_LastTime).count();
	if (m_DeltaTime > 0.1f)
	{
		m_DeltaTime = 0.02f; //prevents deltatime to rise too high when debugging
	}
	m_LastTime = currentTime;
	m_Lag += m_DeltaTime;
	m_DeltaTime *= m_TimeScale;
}

void Timer::Sleep()
{
	auto sleepTime = duration_cast<duration<float>>(m_LastTime + milliseconds(MS_PER_FRAME) - high_resolution_clock::now());
	std::this_thread::sleep_for(sleepTime);
}