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

	int lua_GetPosition(lua_State* pState);
	int lua_SetPosition(lua_State* pState);
};
