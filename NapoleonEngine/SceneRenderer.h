#pragma once
#include "System.h"

#include <array>
#include <memory>

enum class Layer;
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
	TextRendererSystem(Coordinator* const pRegistry);

	void Update(ComponentManager* const pComponentManager) override;

protected:
	std::shared_ptr<System> Clone() const override { return std::make_shared<TextRendererSystem>(*this); };
};

class LayeredRendererSystem : public System //TODO Add Layering
{
public:
	LayeredRendererSystem(Coordinator* const pRegistry);

	void Update(ComponentManager* const pComponentManager) override;

protected:
	std::shared_ptr<System> Clone() const override { return std::make_shared<LayeredRendererSystem>(*this); }
};