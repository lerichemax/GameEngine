#pragma once
#include "Component.h"
#include "Subject.h"

#include "Qube.h"

class Character : public empire::Component
{

public:
	Character();

	virtual void Update() = 0; 
	virtual void Move(ConnectionDirection direction) = 0;
	virtual void Die() = 0;

	empire::Subject* const GetSubject() const { return m_pSubject; }
	Qube* const GetCurrentQube() const { return m_pCurrentQube; }

	void SetCurrentQube(Qube* pTargetQube);
protected:
	Qube* m_pCurrentQube;

	empire::Subject* const m_pSubject;
};
