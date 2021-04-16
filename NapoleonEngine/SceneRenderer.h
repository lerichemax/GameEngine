#pragma once
#include <array>

namespace empire
{
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
		
		unsigned int static const MAX_NBR_LAYERS{ 5 };

		std::array<std::vector<RendererComponent*>, MAX_NBR_LAYERS> m_pLayersGroup;
		
	};
}

