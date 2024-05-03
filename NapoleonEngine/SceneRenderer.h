#pragma once
#include "System.h"
#include <array>

enum class Layer;
class RendererComponent;
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

class LayeredRendererSystem : public System
{
public:
	void Update(ComponentManager* const pComponentManager) override;
};