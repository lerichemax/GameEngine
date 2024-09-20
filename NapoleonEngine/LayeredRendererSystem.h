#pragma once
#include "System.h"

class LayeredRendererSystem : public System
{
public:
	void Update() override;

	void SetSignature() const override;

protected:
	void AddEntity(Entity entity) override;

private:
	std::vector<Entity> m_EntityPerLayer;
};