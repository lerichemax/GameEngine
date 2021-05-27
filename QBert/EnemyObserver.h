#pragma once
#include "Observer.h"

enum class EnemyEvents
{
	CoilyDies,
	SlickSamDies,
	WrongWayDies
};

class EnemyManager;
class EnemyObserver final : public empire::Observer
{
public:
	EnemyObserver(EnemyManager* const pManager); 
	~EnemyObserver() = default;

	void Notify(empire::Component* object, int event) override;

private:
	EnemyManager* const m_pEnemyManager;
};