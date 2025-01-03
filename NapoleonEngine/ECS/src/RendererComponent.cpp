#include "PCH.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

#include "Texture2D.h"

RendererComponent::RendererComponent()
	:Layer{0},
	pTexture{ nullptr },
	pShape{nullptr}
{

}

void RendererComponent::SetShape(std::unique_ptr<geo::Shape> pNewShape)
{
	pShape = std::move(pNewShape);
} 
