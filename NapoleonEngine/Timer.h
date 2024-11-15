#pragma once
#include <chrono>

class ITimer
{
public:
	virtual ~ITimer() = default;
	virtual float GetDeltaTime() const = 0;
	virtual void SetTimeScale(float scale) = 0;
	virtual int GetFPS() const = 0;
};

class TimerLocator final
{
public:
	static ITimer* Get() { return m_pTimerService; }
	static void RegisterTimer(ITimer* const pTimerService) { m_pTimerService = pTimerService; }

private:
	static ITimer* m_pTimerService;
};

class Timer final : public ITimer
{
public:
	float GetDeltaTime() const override { return m_DeltaTime; }
	void SetTimeScale(float scale) override { m_TimeScale = scale; }
	int GetFPS() const override{ return m_FPS; }

private:
	friend class NapoleonEngine;
		
	Timer();
	void Update();

	static const int MS_PER_FRAME = 16; //16 for 60 fps, 33 for 30 fps

	std::chrono::steady_clock::time_point m_LastTime;

	float m_Lag;
	float m_DeltaTime;
	float m_TimeScale;
	int m_FPS;
	void Sleep();
		
};
