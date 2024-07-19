#pragma once
#include "BehaviourComponent.h"

class QBert;
class CharacterController : public BehaviourComponent
{
public:
	void Serialize(StreamWriter& writer) const override;

protected:
	void Initialize() override;
	void Update() override;

private:
	std::shared_ptr<QBert> m_pQbert;
};
