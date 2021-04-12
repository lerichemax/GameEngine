#pragma once
#include "Observer.h"

namespace empire {
	class UIObject;
}

enum class PlayerEvent
{
	PlayerDied,
	IncreasePoints,
	JumpOnQube,
	JumpOnDisk,
	JumpOffDisk,
	GameOver
};

class Pyramid;
class PlayerObserver final : public empire::Observer
{
public:
	PlayerObserver(empire::UIObject* const pPoints, empire::UIObject* const pLives, Pyramid* const pPyramid); //temp default
	~PlayerObserver() = default;
	void Notify(empire::Component* object, int event) override;

private:
	empire::UIObject* const m_pPointsCounter;
	empire::UIObject* const m_LivesCounter;
	Pyramid* const m_pPyramid;
};