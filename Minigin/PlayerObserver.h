#pragma once
#include "Observer.h"
#include "UIObject.h"

namespace dae
{
	class PlayerObserver final : public Observer
	{
	public:
		PlayerObserver(dae::UIObject* pPoints);
		~PlayerObserver() = default;
		void Notify(SceneObject* object, Event event) override;
	
	private:
		dae::UIObject* m_pPointsCounter;
	};
}