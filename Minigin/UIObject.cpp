#include "MiniginPCH.h"
#include "UIObject.h"

#include "ResourceManager.h"
#include "TextRendererComponent.h"

using namespace dae;

UIObject::UIObject()
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	AddComponent<TransformComponent>(0.f, 0.f);
	AddComponent<TextRendererComponent>(" ",font);
	
}
void UIObject::Update(float)
{
	GetComponent<TextRendererComponent>().Update();
}