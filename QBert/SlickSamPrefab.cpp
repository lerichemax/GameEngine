#include "PCH.h"
#include "SlickSamPrefab.h"

#include "RendererComponent.h"

#include "SlickSam.h"

SlickSamPrefab::SlickSamPrefab(Qube* pStart)
{
	AddComponent(new empire::RendererComponent());
	auto pSlickSam = new SlickSam{pStart};
	AddComponent(pSlickSam);
	GetTransform()->Scale(1.5f);
}