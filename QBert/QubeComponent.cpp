#include "PCH.h"
#include "QubeComponent.h"

#include "ResourceManager.h"

QubeComponent::QubeComponent()
	:m_pDefaultText(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube.png")),
	m_pIntermediateTexture(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube_Intermediate.png")),
	m_pFlippedTexture(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube_Flipped.png"))

{

}
void QubeComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(QubeComponent).name());
}