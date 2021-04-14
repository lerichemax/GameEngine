#include "PCH.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

#include "CoilyPrefab.h"
#include "Qube.h"
#include "Coily.h"


CoilyPrefab::CoilyPrefab(Qube* pQube, Pyramid* pPyramid)
{
	auto pText = empire::ResourceManager::GetInstance().GetTexture("Coily_Egg_Big.png");
	AddComponent(new empire::RendererComponent(pText));
	auto pEnemyScript = new Coily(pPyramid, pQube);
	AddComponent(pEnemyScript);
	GetTransform()->Scale(1.5f);
}