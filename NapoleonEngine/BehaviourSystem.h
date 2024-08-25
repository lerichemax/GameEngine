#pragma once
#include "System.h"
#include "Coordinator.h"

class BehaviourSystem final : public System
{
public:
	void Initialize();
	void Update() override;

	void SetSignature(Coordinator* const pRegistry) override;

protected:
	void AddEntity(Entity entity) override;

private:
	std::vector<Entity> m_ToBeAdded;
	std::vector<Entity> m_ToBeStarted;

	void UpdateToBeStartedVector(ComponentManager* const pComponentManager);
};
