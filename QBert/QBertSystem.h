#pragma once
#include "System.h"

class Coordinator;
class QBertSystem : public System 
{
public:
	 void SetSignature(Coordinator* const pRegistry) override;

protected:
	void Start() override;
};