#pragma once
#include "Component.h"

#include "System.h"

class TextRendererComponent;
class FPSCounter final : public Component
{
public:
	FPSCounter() = default;

protected:
	FPSCounter* Clone() const override { return new FPSCounter(*this); }
	void Initialize() override;
	void Update() override;
private:
	TextRendererComponent* m_pTextComp;
};

class FPSCounterSystem final : public System
{
public:
	virtual void Update(ComponentManager* const pComponentManager) override;
};