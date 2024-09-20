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
	for (Entity entity : m_Entities)
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


void ScriptingSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(ScriptingSystem)).hash_code()));
}

void ScriptingSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<ScriptComponent>());
	m_pRegistry->SetSystemSignature<ScriptingSystem>(signature);
}