#include "PCH.h"
#include "ScriptingSystem.h"

#include "ScriptComponent.h"

#define SCRIPT_FOLDER "./Scripts/"

typedef int (ScriptingSystem::* mem_func)(lua_State* L);

template <mem_func func>
int dispatch(lua_State* L) {
	ScriptingSystem* ptr = *static_cast<ScriptingSystem**>(lua_getextraspace(L));
	return ((*ptr).*func)(L);
}


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

	*static_cast<ScriptingSystem**>(lua_getextraspace(m_pState)) = this;

	lua_register(m_pState, "GetPosition", &dispatch<&ScriptingSystem::lua_GetPosition>);
	lua_register(m_pState, "SetPosition", &dispatch<&ScriptingSystem::lua_SetPosition>);
}


void ScriptingSystem::Update()
{
	auto view = m_pRegistry->GetView<ScriptComponent>();

	std::string file{};

	for (Entity entity : view)
	{
		auto* const pScript = m_pRegistry->GetComponent<ScriptComponent>(entity);

		if (!pScript->IsActive())
		{
			return;
		}

		file = SCRIPT_FOLDER + pScript->ScriptFile + ".lua";

		luaL_dofile(m_pState, file.c_str());
		lua_getglobal(m_pState, "update");
		if (lua_isfunction(m_pState, -1))
		{
			lua_pushinteger(m_pState, entity);
			lua_pushnumber(m_pState, Timer::Get().GetDeltaTime());

			if (lua_pcall(m_pState, 2, 0, 0) != LUA_OK) {

				LOG_ERROR("Lua error: %s", lua_tostring(m_pState, -1));
			}
			lua_pop(m_pState, 1);
		}
	}
}

//int ScriptingSystem::lua_GetTransformComponent(lua_State* pState)
//{
//	Entity entity = static_cast<Entity>(luaL_checkinteger(pState, 1));
//
//	auto* pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
//
//	if (!IS_VALID(pTransform))
//	{
//		lua_pushnil(pState);
//		return 1;
//	}
//
//	TransformComponent** pLuaTransform = (TransformComponent**)lua_newuserdata(pState, sizeof(pTransform));
//	pLuaTransform = &pTransform;
//
//	luaL_getmetatable(pState, "TransformMetaTable");
//	lua_setmetatable(pState, -2);
//
//	return 1;
//}

int ScriptingSystem::lua_GetPosition(lua_State* pState)
{
	Entity entity = static_cast<Entity>(luaL_checkinteger(pState, 1));

	glm::vec2 location = m_pRegistry->GetComponent<TransformComponent>(entity)->GetLocation();

	lua_pushnumber(m_pState, location.x);
	lua_pushnumber(m_pState, location.y);

	return 2;
}

int ScriptingSystem::lua_SetPosition(lua_State* pState)
{
	Entity entity = static_cast<Entity>(luaL_checkinteger(pState, 1));
	float x = static_cast<float>(luaL_checknumber(pState, 2));
	float y = static_cast<float>(luaL_checknumber(pState, 3));

	m_pRegistry->GetComponent<TransformComponent>(entity)->SetLocation(x, y);

	return 0;
}