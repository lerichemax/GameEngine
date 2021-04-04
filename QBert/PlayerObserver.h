#pragma once
#include "Observer.h"

namespace empire {
	class UIObject;
}

enum class PlayerEvent
{
	PlayerDied,
	IncreasePoints
};

class PlayerObserver final : public empire::Observer
{
public:
	PlayerObserver(empire::UIObject* const pPoints, empire::UIObject* const pLives);
	~PlayerObserver() = default;
	void Notify(empire::Component* object, int event) override;

private:
	empire::UIObject* const m_pPointsCounter;
	empire::UIObject* const m_LivesCounter;
};