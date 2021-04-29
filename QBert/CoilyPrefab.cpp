#include "PCH.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

#include "CoilyPrefab.h"
#include "Qube.h"
#include "Coily.h"


CoilyPrefab::CoilyPrefab(Qube* pQube, Pyramid* pPyramid)
{
	auto pText = empire::ResourceManager::GetInstance().GetTexture("Textures/Enemies/Coily/Coily_Egg_Small.png");
	AddComponent(new empire::RendererComponent(pText, empire::Layer::middleground));
	auto pEnemyScript = new Coily(pPyramid, pQube);
	AddComponent(pEnemyScript);
	GetTransform()->Scale(1.5f);
	
}