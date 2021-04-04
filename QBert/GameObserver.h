#pragma once
#include "Observer.h"
#include "GameController.h"

enum class GameEvents
{
	QubeFlipped
};

class GameObserver final : public empire::Observer
{
public:
	GameObserver(GameController* const pController);
	~GameObserver() = default;
	void Notify(empire::Component * object, int event) override;
private:
	GameController const * const m_pController;
};