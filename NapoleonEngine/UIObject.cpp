#include "PCH.h"
#include "UIObject.h"

#include "ResourceManager.h"
#include "TextRendererComponent.h"

using namespace empire;

UIObject::UIObject() : GameObject()
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	AddComponent(new TextRendererComponent(" ",font));
	
}
void UIObject::Update(float)
{
	GetComponent<TextRendererComponent>()->Update();
}