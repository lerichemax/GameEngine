#pragma once
#include "Component.h"

namespace empire
{
	class TextRendererComponent;
	class FPSCounter final : public Component
	{
	public:
		FPSCounter() = default;
		~FPSCounter() = default;

		FPSCounter* Clone() override { return new FPSCounter(*this); }
		
		void Initialize() override;
		void Update() override;
	private:
		TextRendererComponent* m_pTextComp;

		
	};
}