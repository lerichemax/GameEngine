#pragma once
#include "System.h"
#include "lua.hpp"

class ScriptingSystem : public System
{
public:
	~ScriptingSystem();

protected:
	void Initialize() override;
	void Update() override;

private: 
	lua_State* m_pState;
};
