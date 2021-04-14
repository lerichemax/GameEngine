#pragma once
#include "Observer.h"

enum class EnemyEvents
{
	CoilyDies,
	SlickSamDies,
	WrongWayDies
};

class Pyramid;
class EnemyObserver final : public empire::Observer
{
public:
	EnemyObserver(Pyramid* const pPyramid); 
	~EnemyObserver() = default;

	void Notify(empire::Component* object, int event) override;

private:
	Pyramid* const m_pPyramid;
};