#include "MiniginPCH.h"
#include "GameObject.h"

using namespace dae;
GameObject::GameObject(RendererComponent* pRenderComp)
	:SceneObject()
{
	AddComponent<RendererComponent>(pRenderComp);
}

GameObject::~GameObject() = default;

void GameObject::Update(){}