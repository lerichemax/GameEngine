#pragma once
#include "System.h"
#include "lua.hpp"

class ScriptingSystem : public System
{
public:
	~ScriptingSystem();

	void Serialize(StreamWriter& writer) const override;
	void SetSignature() override;

protected:
	void Initialize() override;
	void Update() override;

private: 
	lua_State* m_pState;
};
