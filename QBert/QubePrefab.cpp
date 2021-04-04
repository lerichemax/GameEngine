#include "PCH.h"
#include "QubePrefab.h"
#include "RendererComponent.h"
#include "Qube.h"
#include "ResourceManager.h"

QubePrefab::QubePrefab() : GameObject()
{
	auto text = ResourceManager::GetInstance().LoadTexture("Qube.png");
	auto flippedText = ResourceManager::GetInstance().LoadTexture("Qube_Flipped.png");
	GetTransform()->Scale(1.75f);
	AddComponent(new Qube(text, flippedText));
	AddComponent(new RendererComponent(text));
}
