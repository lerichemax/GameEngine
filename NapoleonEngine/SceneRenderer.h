#pragma once
#include "System.h"

#include <array>
#include <memory>

enum class Layer : uint8_t;
class RendererComponent;
class Coordinator;
class SceneRenderer final
{
public:
	SceneRenderer() = default;

	void Render();
	void AddToGroup(RendererComponent* pRenderer, Layer layer);
	void RemoveFromGroup(RendererComponent* pRenderer, Layer layer);
	
private:
		
	unsigned int static const MAX_NBR_LAYERS{ 8 };

	std::array<std::vector<RendererComponent*>, MAX_NBR_LAYERS> m_pLayersGroup;
};

class TextRendererSystem : public System
{
public:
	void Update(ComponentManager* const pComponentManager) override;

	void SetSignature(Coordinator* const pRegistry) override;
};

class LayeredRendererSystem : public System
{
public:
	void Update(ComponentManager* const pComponentManager) override;

	void SetSignature(Coordinator* const pRegistry) override;

protected:
	void AddEntity(Entity entity) override;

private:
	std::vector<Entity> m_EntityPerLayer;

};