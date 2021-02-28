#pragma once
#include "Observer.h"

namespace dae
{
	class PlayerObserver final : public Observer
	{
	public:
		PlayerObserver() = default;
		void Notify(const SceneObject& object, Event event) override;
	
	private:
	};
}