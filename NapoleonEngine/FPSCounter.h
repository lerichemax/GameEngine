#pragma once
#include "GameObject.h"

namespace empire
{
	class FPSCounter final : public GameObject
	{
	public:
		FPSCounter();
		~FPSCounter() = default;

		void Update(float deltaTime) override;
	};
}