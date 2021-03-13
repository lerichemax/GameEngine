#pragma once
#include "Observer.h"

namespace empire
{
	class UIObject;
	class PlayerObserver final : public Observer
	{
	public:
		PlayerObserver(empire::UIObject* const pPoints, empire::UIObject* const pLives);
		~PlayerObserver() = default;
		void Notify(Component* object, Event event) override;
	
	private:
		empire::UIObject* const m_pPointsCounter;
		empire::UIObject* const m_LivesCounter;
	};
}