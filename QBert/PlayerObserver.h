#pragma once
#include "Observer.h"

namespace empire {
	class TextRendererComponent;
	
}

enum class PlayerEvent
{
	PlayerDied,
	PlayerJumpOut,
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
	PlayerObserver(empire::TextRendererComponent* const pPoints, empire::TextRendererComponent* const pLives, Pyramid* const pPyramid); 
	~PlayerObserver() = default;
	void Notify(empire::Component* object, int event) override;

private:
	empire::TextRendererComponent* const m_pPointsCounter;
	empire::TextRendererComponent* const m_LivesCounter;
	Pyramid* const m_pPyramid;
};