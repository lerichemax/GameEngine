#pragma once
#include "System.h"

class Coordinator;
class QBertSystem : public System 
{
public:
	 void SetSignature() override;

	 void Serialize(StreamWriter& writer) const override;

	 void Reset(bool fullReset, Entity targetQubeEntity);

protected:
	void Start() override;

};