#include "PCH.h"
#include "ScriptingSystem.h"

#include "ScriptComponent.h"

ScriptingSystem::~ScriptingSystem()
{
	if (IS_VALID(m_pState))
	{
		lua_close(m_pState);
	}
}

void ScriptingSystem::Initialize()
{
	m_pState = luaL_newstate();
	luaL_openlibs(m_pState);
}


void ScriptingSystem::Update()
{
	auto view = m_pRegistry->GetView<ScriptComponent>();

	for (Entity entity : view)
	{
		auto* const pScript = m_pRegistry->GetComponent<ScriptComponent>(entity);

		if (!pScript->IsActive())
		{
			return;
		}

		luaL_dofile(m_pState, pScript->ScriptFile.c_str());
		lua_getglobal(m_pState, "update");
		if (lua_isfunction(m_pState, -1))
		{
			lua_pcall(m_pState, 0, 0, 0);
			lua_pop(m_pState, 1);
		}
	}
}
