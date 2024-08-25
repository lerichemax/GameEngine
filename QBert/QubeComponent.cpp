#include "PCH.h"
#include "QubeComponent.h"

#include "ResourceManager.h"

QubeComponent::QubeComponent()
	:pDefaultText(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube.png")),
	pIntermediateTexture(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube_Intermediate.png")),
	pFlippedTexture(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube_Flipped.png"))

{

}
void QubeComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(QubeComponent).name());
}