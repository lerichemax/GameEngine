#include "PCH.h"
#include "QubePrefab.h"
#include "RendererComponent.h"
#include "Qube.h"
#include "ResourceManager.h"

QubePrefab::QubePrefab() : GameObject()
{
	auto text = ResourceManager::GetInstance().GetTexture("Qube.png");
	auto interText = ResourceManager::GetInstance().GetTexture("Qube_Intermediate.png");
	auto flippedText = ResourceManager::GetInstance().GetTexture("Qube_Flipped.png");
	GetTransform()->Scale(1.75f);
	AddComponent(new Qube(text, interText, flippedText));
	AddComponent(new RendererComponent(text));
}
