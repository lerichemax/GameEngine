#pragma once
#include "Observer.h"

namespace dae
{
	class UIObject;
	class PlayerObserver final : public Observer
	{
	public:
		PlayerObserver(dae::UIObject* const pPoints, dae::UIObject* const pLives);
		~PlayerObserver() = default;
		void Notify(Component* object, Event event) override;
	
	private:
		dae::UIObject* const m_pPointsCounter;
		dae::UIObject* const m_LivesCounter;
	};
}