#pragma once
#include "System.h"

#include <array>
#include <memory>

class Coordinator;

class TextRendererSystem : public System
{
public:
	void Update() override;

	void SetSignature() override;
};

class LayeredRendererSystem : public System
{
public:
	void Update() override;

	void SetSignature() override;

protected:
	void AddEntity(Entity entity) override;

private:
	std::vector<Entity> m_EntityPerLayer;
};