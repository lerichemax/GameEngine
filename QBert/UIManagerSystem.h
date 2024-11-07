#pragma once
#include "System.h"

class TextRendererComponent;
class UIManagerSystem : public System
{
public:
	UIManagerSystem() = default;
	~UIManagerSystem() = default;

protected:
	void Start() override;

private:
	Entity m_UiEntity{NULL_ENTITY};
};