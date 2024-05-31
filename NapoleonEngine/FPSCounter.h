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

struct FPSCounterComponent : ECS_Component
{
	//empty class ?
};

class FPSCounterSystem : public System
{
public:
	FPSCounterSystem(Coordinator* const pRegistry);

	virtual void Update(ComponentManager* const pComponentManager) override;


protected:
	std::shared_ptr<System> Clone() const override { return std::make_shared<FPSCounterSystem>(*this); }
};