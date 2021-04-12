#include "PCH.h"
#include "DiskPrefab.h"
#include "RendererComponent.h"
#include "ResourceManager.h"
#include "ColoredDisk.h"

DiskPrefab::DiskPrefab(Qube* top)
{
	auto const diskText = empire::ResourceManager::GetInstance().GetTexture("Disk.png");
	AddComponent(new empire::RendererComponent{ diskText });
	AddComponent(new ColoredDisk{ top });
	GetTransform()->Scale(2);
}