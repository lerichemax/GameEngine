#pragma once
#include "System.h"

class Coordinator;
class UiSystem : public System
{
public:
	void Update() override;

	void SetSignature() override;
};