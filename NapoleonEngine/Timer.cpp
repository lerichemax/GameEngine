#include "PCH.h"
#include "Timer.h"

#include <thread>

ITimer* TimerLocator::m_pTimerService = nullptr;

using namespace std::chrono;

Timer::Timer()
	:m_LastTime(high_resolution_clock::now()),
	m_Lag(0.f),
	m_DeltaTime(0.f),
	m_TimeScale(1.f),
	m_FPS(0)
	
{
}

void Timer::Update()
{
	const auto currentTime = high_resolution_clock::now();
	m_DeltaTime =  duration<float>(currentTime - m_LastTime).count();
	if (m_DeltaTime > 0.1f)
	{
		m_DeltaTime = 0.02f; //prevents deltatime from rising too high when debugging
	}
	m_LastTime = currentTime;
	m_Lag += m_DeltaTime;
	m_FPS = static_cast<int>(1 / m_DeltaTime);

	m_DeltaTime *= m_TimeScale;
}

void Timer::Sleep()
{
	auto sleepTime = duration_cast<duration<float>>(m_LastTime + milliseconds(MS_PER_FRAME) - high_resolution_clock::now());
	std::this_thread::sleep_for(sleepTime);
}